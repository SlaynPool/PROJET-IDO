/*
   ----------------------------------------------------------------------------
   "THE PROP-WARE LICENSE" (Revision 42):
   <https://github.com/JyeSmith> wrote this file.  As long as you retain this notice you
   can do whatever you want with this stuff. If we meet some day, and you think
   this stuff is worth it, you can buy me some props in return.   Jye Smith
   ----------------------------------------------------------------------------
*/

/* Some of the below code is taken from examples provided by Felix on RCGroups.com

   KISS ESC 24A Serial Example Code for Arduino.
   https://www.rcgroups.com/forums/showthread.php?2555162-KISS-ESC-24A-Race-Edition-Flyduino-32bit-ESC
   https://www.rcgroups.com/forums/showatt.php?attachmentid=8521072&d=1450345654
*/

#include <HardwareSerial.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "Arduino.h"
#include "esp32-hal.h"

#include "Filters.h"

/////////Filter Bank////////

LPF FLTR_VOLT_SLOW(4);
LPF FLTR_MA_SLOW(4);
LPF FLTR_RPM_SLOW(4);
LPF FLTR_KV_SLOW(4);

LPF FLTR_VOLT_FAST(2);
LPF FLTR_MA_FAST(2);
LPF FLTR_RPM_FAST(2);
LPF FLTR_KV_FAST(2);

////////////////////////////


#define MOTOR_POLES 14

rmt_data_t dshotPacket[16];
rmt_obj_t* rmt_send = NULL;

HardwareSerial MySerial(1);


uint32_t newtime;
uint8_t receivedBytes = 0;
bool requestTelemetry = false;
uint16_t dshotUserInputValue = 0;

uint16_t dshotmin = 48;
uint16_t dshotmax = 2047;
uint16_t dshotidle = dshotmin + round(3.5 * (dshotmax - dshotmin) / 100); // 3.5%
uint16_t dshot50 =   dshotmin + round(50 * (dshotmax - dshotmin) / 100); // 50%
uint16_t dshot75 =   dshotmin + round(75 * (dshotmax - dshotmin) / 100); // 75%

int16_t ESC_telemetrie[5]; // Temperature, Voltage, Current, used mAh, eRpM
uint8_t temperatureMax = 100;
uint8_t temperature = 0;
uint8_t tempSlowFilter = 0;
float voltage = 0;
float voltageMin = 99;
uint32_t current = 0;
uint32_t currentSlowFilter = 0;
uint32_t erpm = 0;
uint32_t eprmSlowFilter = 0;
uint32_t rpm = 0;
uint32_t rpmMAX = 0;
uint32_t kv = 0;

uint32_t kvMax = 0;
void resetMaxMinValues() {
  gotTouch4();
}
void gotTouch8(){
    dshotUserInputValue = 0;
    } // DIGITAL_CMD_MOTOR_STOP
void gotTouch9(){
    dshotUserInputValue = 247;
    resetMaxMinValues();
    } // 10%
void gotTouch7(){
    dshotUserInputValue = 447;
    resetMaxMinValues();
    } // 20%
void gotTouch6(){
    dshotUserInputValue = 1047;
    resetMaxMinValues();
    } // 50%
void gotTouch5(){ 
    dshotUserInputValue = 2047;                 
    resetMaxMinValues();
    } // 100%
void gotTouch4(){ 
    temperatureMax = 0;
    voltageMin = 99;
    current = 0;
    erpm = 0;
    rpm= 0;
    kv = 0;

uint32_t kvSlowFilter = 0;

bool doTLM = true;

void clearValues() {
  FLTR_VOLT_SLOW.clear();
  FLTR_MA_SLOW.clear();
  FLTR_RPM_SLOW.clear();
  FLTR_KV_SLOW.clear();

  FLTR_VOLT_FAST.clear();
  FLTR_MA_FAST.clear();
  FLTR_RPM_FAST.clear();
  FLTR_KV_FAST.clear();
}

void gotTouch8() {
  dshotUserInputValue = 0;
  runMQTBSequence = false;
  printTelemetry = true;
  doTLM = false;
} // DIGITAL_CMD_MOTOR_STOP

void gotTouch9() {
  dshotUserInputValue = 247;
  resetMaxMinValues();
  runMQTBSequence = false;
  printTelemetry = true;
  doTLM = true;
} // 10%

void gotTouch7() {
  dshotUserInputValue = 447;
  resetMaxMinValues();
  runMQTBSequence = false;
  printTelemetry = true;
  doTLM = true;
} // 20%

void gotTouch6() {
  dshotUserInputValue = 1047;
  resetMaxMinValues();
  runMQTBSequence = false;
  printTelemetry = true;
  doTLM = true;
} // 50%

void gotTouch5() {
  dshotUserInputValue = 2047;
  resetMaxMinValues();
  runMQTBSequence = false;
  printTelemetry = true;
  doTLM = true;
} // 100%

void gotTouch4() {
  tempSlowFilter = 0;
  voltageMin = 99;
  currentSlowFilter = 0;
  eprmSlowFilter = 0;
  rpmMAX = 0;
  kvSlowFilter = 0;
  runMQTBSequence = false;
  printTelemetry = true;
  clearValues();

}
void resetMaxMinValues() {
  gotTouch4();
}


void IRAM_ATTR getTelemetry() {
  requestTelemetry = true;
}

void startTelemetryTimer() {
  timer = timerBegin(0, 80, true); // timer_id = 0; divider=80; countUp = true;
  timerAttachInterrupt(timer, &getTelemetry, true); // edge = true
  timerAlarmWrite(timer, 20000, true);  //1000 = 1 ms
  timerAlarmEnable(timer);
}

// Second core used to handle dshot packets
void secondCoreTask( void * pvParameters ) {
  while (1) {

    dshotOutput(dshotUserInputValue, requestTelemetry);

    if (requestTelemetry) {
      requestTelemetry = false;
      receivedBytes = 0;
    }

    vTaskDelay(1);

  }
}

void setup() {

  Serial.begin(115200);
  MySerial.begin(115200, SERIAL_8N1, 16, 17);

    if ((rmt_send = rmtInit(5, true, RMT_MEM_64)) == NULL) {
        Serial.println("init sender failed\n");
    }

    float realTick = rmtSetTick(rmt_send, 12.5); // 12.5ns sample rate
    Serial.printf("rmt_send tick set to: %fns\n", realTick);

   
    
    // Output disarm signal while esc initialises and do some display stuff.
    uint8_t xbeep = random(15, 100);
    uint8_t ybeep = random(15, 50);
    uint8_t ibeep = 0;
    while (millis() < 3500) {
        dshotOutput(0, false);
        delay(1);  
        
        display.clear();            
        ibeep++; 
        if (ibeep == 100) {
            ibeep = 0;
            xbeep = random(15, 50);
            ybeep = random(15, 50);
        }
        display.drawString(xbeep, ybeep, "beep");
        if (millis() < 500) {         
            Serial.println( "Initialising ESC... 4s");
        } else if (millis() < 1500) {   
            Serial.println( "Initialising ESC... 3s");
        } else if (millis() < 2500) {  
            Serial.println( "Initialising ESC... 2s");
        } else {                
            Serial.println( "Initialising ESC... 1s");
        }
        display.display(); 
    }
    
    touchAttachInterrupt(T4, gotTouch4, 40);
    touchAttachInterrupt(T5, gotTouch5, 40);
    touchAttachInterrupt(T6, gotTouch6, 40);
    touchAttachInterrupt(T7, gotTouch7, 40);
    touchAttachInterrupt(T8, gotTouch8, 40);
    touchAttachInterrupt(T9, gotTouch9, 40);

    // Empty Rx Serial of garbage telemtry
    while(MySerial.available())
        MySerial.read();

    newtime = millis() + 1; 
    
    requestTelemetry = true;
    BeginWebUpdate();

  loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  loadcell.set_scale(LOADCELL_CALIBRATION);
  loadcell.tare();

  if ((rmt_send = rmtInit(5, true, RMT_MEM_64)) == NULL) {
    Serial.println("init sender failed\n");
  }

  float realTick = rmtSetTick(rmt_send, 12.5); // 12.5ns sample rate
  Serial.printf("rmt_send tick set to: %fns\n", realTick);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Output disarm signal while esc initialises and do some display stuff.
  uint8_t xbeep = random(15, 100);
  uint8_t ybeep = random(15, 50);
  uint8_t ibeep = 0;
  while (millis() < 1500) {
    dshotOutput(0, false);
    delay(1);

    display.clear();
    ibeep++;
    if (ibeep == 100) {
      ibeep = 0;
      xbeep = random(15, 50);
      ybeep = random(15, 50);
    }
    display.drawString(xbeep, ybeep, "beep");
    if (millis() < 500) {
      Serial.println( "Initialising ESC... 4s");
    } else if (millis() < 1500) {
      Serial.println( "Initialising ESC... 3s");
    } else if (millis() < 2500) {
      Serial.println( "Initialising ESC... 2s");
    } else {
      Serial.println( "Initialising ESC... 1s");
    }
    display.display();
  }

  touchAttachInterrupt(T4, gotTouch4, 40);
  touchAttachInterrupt(T5, gotTouch5, 40);
  touchAttachInterrupt(T6, gotTouch6, 40);
  touchAttachInterrupt(T7, gotTouch7, 40);
  touchAttachInterrupt(T8, gotTouch8, 40);
  touchAttachInterrupt(T9, gotTouch9, 40);

  // Empty Rx Serial of garbage telemtry
  while (MySerial.available())
    MySerial.read();

  requestTelemetry = true;

  BeginWebUpdate();

  startTelemetryTimer(); // Timer used to request tlm continually in case ESC rcv bad packet

  xTaskCreatePinnedToCore(secondCoreTask, "Task1", 10000, NULL, 1, &Task1, 0);

  Serial.print("Time (ms)");
  Serial.print(",");
  Serial.print("dshot");
  Serial.print(",");
  Serial.print("Voltage (V)");
  Serial.print(",");
  Serial.print("Current (A)");
  Serial.print(",");
  Serial.print("RPM");
  Serial.print(",");
  Serial.println("Thrust (g)");

#ifdef MINIQUADTESTBENCH
  dshotUserInputValue = dshotidle;
  runMQTBSequence = true;
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
 Serial.println( "Running MQTB Sequence...");
  display.display();
#endif

  clearValues();

}

void loop() {

  HandleWebUpdate();

  if (loadcell.is_ready()) {
    thrust = loadcell.get_units(1);
  }

  if (!requestTelemetry) {
    receiveTelemtrie();
  }


    if (millis() > newtime) {
 
        if (requestTelemetry) {                
            dshotOutput(dshotUserInputValue, true);
            requestTelemetry = false;
            receivedBytes = 0;
            updateDisplay();
  
        } else {
            dshotOutput(dshotUserInputValue, false);
            receiveTelemtrie();
        }

        newtime ++;        
    }

}

void updateDisplay() {    
   
    Serial.print( "Dshot Packet");
    Serial.print( "Temp C");
    Serial.print( "Volt");
    Serial.print( "mA");
    Serial.print("eRPM");
    Serial.println("KV");
    
    Serial.print( String(temperature));
    Serial.print(String(voltage));
    Serial.print( String(current));
    Serial.print( String(erpm));
    Serial.print(String(kv));
    
    Serial.print(String(dshotUserInputValue));
    Serial.print(String(temperatureMax));
    Serial.print( String(voltageMin));
    Serial.print( String(currentMax));
    Serial.print( String(erpmMax));
    Serial.print( String(kvMax));
    
}

void receiveTelemtrie(){
    static uint8_t SerialBuf[10];
    
    if(receivedBytes < 9){ // collect bytes
    
        while(MySerial.available()){
            SerialBuf[receivedBytes] = MySerial.read();
            receivedBytes++;
        }
        if(receivedBytes == 10){ // transmission complete
          
            uint8_t crc8 = get_crc8(SerialBuf, 9); // get the 8 bit CRC
          
            if(crc8 != SerialBuf[9]) {
                Serial.println("CRC transmission failure");
                return; // transmission failure 
            }
          
            // compute the received values
            ESC_telemetrie[0] = SerialBuf[0]; // temperature
            ESC_telemetrie[1] = (SerialBuf[1]<<8)|SerialBuf[2]; // voltage
            ESC_telemetrie[2] = (SerialBuf[3]<<8)|SerialBuf[4]; // Current
            ESC_telemetrie[3] = (SerialBuf[5]<<8)|SerialBuf[6]; // used mA/h
            ESC_telemetrie[4] = (SerialBuf[7]<<8)|SerialBuf[8]; // eRpM *100
                  
=======
#ifdef MINIQUADTESTBENCH
  if (runMQTBSequence) {
    currentTime = millis();
    if (currentTime >= 4000 && currentTime < 6000) {
      dshotUserInputValue = dshot50;
    } else if (currentTime >= 6000 && currentTime < 8000) {
      dshotUserInputValue = dshotidle;
    } else if (currentTime >= 8000 && currentTime < 10000) {
      dshotUserInputValue = dshot75;
    } else if (currentTime >= 10000 && currentTime < 12000) {
      dshotUserInputValue = dshotidle;
    } else if (currentTime >= 12000 && currentTime < 14000) {
      dshotUserInputValue = dshotmax;
    } else if (currentTime >= 14000 && currentTime < 16000) {
      dshotUserInputValue = dshotmin;
    } else if (currentTime >= 16000 && currentTime < 22000) {
      dshotUserInputValue = dshotmin + (currentTime - 16000) * (dshotmax - dshotmin) / 6000.0;
    } else if (currentTime >= 24000 && currentTime < 26000) {
      dshotUserInputValue = dshotidle;
    } else if (currentTime >= 26000 && currentTime < 28000) {
      printTelemetry = false;
      dshotUserInputValue = 0;
    }
  }
#endif

}

void receiveTelemtrie() {


  static uint8_t SerialBuf[10];

  if (MySerial.available()) {
    SerialBuf[receivedBytes] = MySerial.read();
    receivedBytes++;
  }

  if (receivedBytes > 9) { // transmission complete

    uint8_t crc8 = get_crc8(SerialBuf, 9); // get the 8 bit CRC

    if (crc8 != SerialBuf[9]) {
      //                Serial.println("CRC transmission failure");

      // Empty Rx Serial of garbage telemtry
      while (MySerial.available())
        MySerial.read();

      requestTelemetry = true;

      return; // transmission failure
    }

    // compute the received values
    ESC_telemetrie[0] = SerialBuf[0]; // temperature
    ESC_telemetrie[1] = (SerialBuf[1] << 8) | SerialBuf[2]; // voltage
    ESC_telemetrie[2] = (SerialBuf[3] << 8) | SerialBuf[4]; // Current
    ESC_telemetrie[3] = (SerialBuf[5] << 8) | SerialBuf[6]; // used mA/h
    ESC_telemetrie[4] = (SerialBuf[7] << 8) | SerialBuf[8]; // eRpM *100

    requestTelemetry = true;

    if (!runMQTBSequence) { // Do not update during MQTB sequence.  Slows serial output.
      updateDisplay();
    }


    //      Serial.println("Requested Telemetrie");
    //      Serial.print("Temperature (C): ");
    //      Serial.println(ESC_telemetrie[0]);
    //      Serial.print("Voltage (V): ");
    //      Serial.println(ESC_telemetrie[1] / 100.0);
    //      Serial.print("Current (mA): ");
    //      Serial.println(ESC_telemetrie[2] * 100);
    //      Serial.print("mA/h: ");
    //      Serial.println(ESC_telemetrie[3] * 10);
    //      Serial.print("eRPM : ");
    //      Serial.println(ESC_telemetrie[4] * 100);
    //      Serial.print("RPM : ");
    //      Serial.println(ESC_telemetrie[4] * 100 / 7.0);  // 7 = 14 magnet count / 2
    //      Serial.print("KV : ");
    //      Serial.println( (ESC_telemetrie[4] * 100 / 7.0) / (ESC_telemetrie[1] / 100.0) );  // 7 = 14 magnet count / 2
    //      Serial.println(" ");
    //      Serial.println(" ");

          
            temperature = 0.9*temperature + 0.1*ESC_telemetrie[0];
            if (temperature > temperatureMax) {
                temperatureMax = temperature;
            }
            
            voltage = 0.9*voltage + 0.1*(ESC_telemetrie[1] / 100.0);
            if (voltage < voltageMin) {
                voltageMin = voltage;
            }
            
            current = 0.9*current + 0.1*(ESC_telemetrie[2] * 100);
            if (current > currentMax) {
                currentMax = current;
            }
            
            erpm = 0.9*erpm + 0.1*(ESC_telemetrie[4] * 100);
            if (erpm > erpmMax) {
                erpmMax = erpm;
            }
            
            rpm = erpm / (MOTOR_POLES / 2);
            if (rpm > rpmMAX) {
                rpmMAX = rpm;
            }
            
            if (rpm) {                  // Stops weird numbers :|
                kv = rpm / voltage;
            } else {
                kv = 0;
            }
            if (kv > kvMax) {
                kvMax = kv;
            }

            requestTelemetry = true;
          
        }
    
    }


    if (printTelemetry) {
      Serial.print(millis());
      Serial.print(",");
      Serial.print(dshotUserInputValue);
      Serial.print(",");
      //      Serial.print("Voltage (V): ");
      Serial.print(ESC_telemetrie[1] / 100.0);
      Serial.print(",");
      //      Serial.print("Current (A): ");
      Serial.print(ESC_telemetrie[2] / 10.0);
      Serial.print(",");
      //      Serial.print("RPM : ");
      Serial.print(ESC_telemetrie[4] * 100 / (MOTOR_POLES / 2));
      Serial.print(",");
      // Thrust
      Serial.println(thrust);
    }

    temperature = 0.1 * ESC_telemetrie[0];
    if (temperature > tempSlowFilter) {
      tempSlowFilter = temperature;
    }


    current = FLTR_MA_FAST.update(ESC_telemetrie[2] * 100);
    erpm =  FLTR_RPM_FAST.update(ESC_telemetrie[4] * 100);
    voltage = FLTR_VOLT_FAST.update(ESC_telemetrie[1]);

    rpm = erpm / (MOTOR_POLES / 2);

    if (rpm > 1000) {                  // Stops weird numbers :|
      kv = FLTR_KV_FAST.update(rpm / (voltage / 100.0) / ( (float(dshotUserInputValue) - dshotmin) / (dshotmax - dshotmin)));
    } else {
      kv = 0;
    }

    if (doTLM) {

      currentSlowFilter = FLTR_MA_SLOW.update(current);
      eprmSlowFilter = FLTR_RPM_SLOW.update(erpm);
      voltageMin = FLTR_VOLT_SLOW.update(voltage);
      kvSlowFilter = FLTR_KV_SLOW.update(kv);

    }

  }


  return;

}

uint8_t update_crc8(uint8_t crc, uint8_t crc_seed){
  uint8_t crc_u, i;
  crc_u = crc;
  crc_u ^= crc_seed;
  for ( i=0; i<8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
  return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen){
  uint8_t crc = 0, i;
  for( i=0; i<BufLen; i++) crc = update_crc8(Buf[i], crc);
  return (crc);
}

void dshotOutput(uint16_t value, bool telemetry) {


    uint16_t packet;
    
    // telemetry bit    
    if (telemetry) {
        packet = (value << 1) | 1;

  uint16_t packet;

  // telemetry bit
  if (telemetry) {
    packet = (value << 1) | 1;
  } else {
    packet = (value << 1) | 0;
  }

  // https://github.com/betaflight/betaflight/blob/09b52975fbd8f6fcccb22228745d1548b8c3daab/src/main/drivers/pwm_output.c#L523
  int csum = 0;
  int csum_data = packet;
  for (int i = 0; i < 3; i++) {
    csum ^=  csum_data;
    csum_data >>= 4;
  }
  csum &= 0xf;
  packet = (packet << 4) | csum;

  // durations are for dshot600
  // https://blck.mn/2016/11/dshot-the-new-kid-on-the-block/
  // Bit length (total timing period) is 1.67 microseconds (T0H + T0L or T1H + T1L).
  // For a bit to be 1, the pulse width is 1250 nanoseconds (T1H – time the pulse is high for a bit value of ONE)
  // For a bit to be 0, the pulse width is 625 nanoseconds (T0H – time the pulse is high for a bit value of ZERO)
  for (int i = 0; i < 16; i++) {
    if (packet & 0x8000) {
      dshotPacket[i].level0 = 1;
      dshotPacket[i].duration0 = 100;
      dshotPacket[i].level1 = 0;
      dshotPacket[i].duration1 = 34;

    } else {
      dshotPacket[i].level0 = 1;
      dshotPacket[i].duration0 = 50;
      dshotPacket[i].level1 = 0;
      dshotPacket[i].duration1 = 84;
    }
    packet <<= 1;
  }

  rmtWrite(rmt_send, dshotPacket, 16);

  return;

}


uint8_t update_crc8(uint8_t crc, uint8_t crc_seed) {
  uint8_t crc_u, i;
  crc_u = crc;
  crc_u ^= crc_seed;
  for ( i = 0; i < 8; i++) crc_u = ( crc_u & 0x80 ) ? 0x7 ^ ( crc_u << 1 ) : ( crc_u << 1 );
  return (crc_u);
}

uint8_t get_crc8(uint8_t *Buf, uint8_t BufLen) {
  uint8_t crc = 0, i;
  for ( i = 0; i < BufLen; i++) crc = update_crc8(Buf[i], crc);
  return (crc);
}
/*
void updateDisplay() {
  display.clear();

  display.setTextAlignment(TEXT_ALIGN_LEFT);
 Serial.println( "Dshot Packet");
  display.drawString(0, 10, "Temp C");
  display.drawString(0, 20, "Volt");
  display.drawString(0, 30, "mA");
  display.drawString(0, 40, "eRPM");
  display.drawString(0, 50, "KV");

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(80, 10, String(temperature));
  display.drawString(80, 20, String(float(voltage) / 100.0));
  display.drawString(80, 30, String(current));
  display.drawString(80, 40, String(erpm));
  display.drawString(80, 50, String(kv));

  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128,  0, String(dshotUserInputValue));
  display.drawString(128, 10, String(tempSlowFilter));
  display.drawString(128, 20, String(float(voltageMin) / 100.0));
  display.drawString(128, 30, String(currentSlowFilter));
  display.drawString(128, 40, String(eprmSlowFilter));
  display.drawString(128, 50, String(kvSlowFilter));

  display.display();
}*/

#include <DShot.h>
#define DSHOT_PORT D7


DShot esc1;

uint16_t throttle = 0;
uint16_t target = 0;

void setup() {
  Serial.begin(115200);

  // Notice, all pins must be connected to same PORT
  esc1.attach(7);
  esc1.setThrottle(throttle);
  Serial.print("Fin setup");
}


void loop(){
  esc1.setThrottle(5000);
  delay(10);
    
}

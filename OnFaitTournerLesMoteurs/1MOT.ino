#include <Servo.h>

Servo motA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motA.attach(6,1000,2000);
  char olddata;
  olddata=48;
  motA.write(0);
  delay(5000);
  motA.write(0);

}

void loop() {
  
  // put your main code here, to run repeatedly:
  //Serial.println("Sending Throttle");
 // if(Serial.available()){
  //  char olddata;
  //  char data;
   
    //data=Serial.read();

    //if(data != olddata){
      //  olddata = data;
        motA.write(90);
        delay(200);
   //   }    
      
  }
  

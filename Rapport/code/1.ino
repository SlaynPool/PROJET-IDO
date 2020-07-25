#include <Servo.h>

// On va definir le motA de type Servo. 
Servo motA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // On relie le motA au pin 5 avec 1000usec min et 2000usec maximum de periode.
  motA.attach(5,1000,2000); 
  delay(5000);
  // On ecrit sur le motA 0 comme angle de rotation du moteur donc ici on le stop.
  motA.write(0);
  }


void loop() {
  // put your main code here, to run repeatedly:
  motA.write(0);
  delay(10000);
  //On fait tourner notre moteur au minimum
  motA.write(12);
  delay(2500);
  motA.write(50);
  delay(2500);
  //On fait tourner notre moteur au maximum
  motA.write(84);
  delay(2500);  
  }

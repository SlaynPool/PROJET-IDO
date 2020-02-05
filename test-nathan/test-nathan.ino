/* pin 11,5,6,10 utilisé
commence a 12 de throttle OUI

liste des pin
Bleu(avant) droit = 12
Bleu(avant) gauche = 11
Noir(arriere) droit = 6
Noir(arriere) gauche = 5


*/

#include <Servo.h>

Servo motA;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  motA.attach(11,1000,2000);
  motB.attach(5,1000,2000);
  motC.attach(6,1000,2000);
  motD.attach(10,1000,2000);

  char olddata;
  olddata=48;
  delay(5000);
  motA.write(0);
  motB.write(0);
  motC.write(0);
  motD.write(0);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  //Serial.println("Sending Throttle");
  if(Serial.available()){
  //  char olddata;
    char data;
    data=Serial.read();
    switch (data) {
        case 48 : Serial.println("On tourne pas");
                  motA.write(0);
                  motB.write(0);
                  motC.write(0);
                  motD.write(0);
    break;
        case 49 : Serial.println("On tourne a 12");
                  motA.write(12);
                  motB.write(12);
                  motC.write(12);
                  motD.write(12);
    break;
            case 50 : Serial.println("On tourne a 24");
                  motA.write(24);
                  motB.write(24);
                  motC.write(24);
                  motD.write(24);
    break;
            case 51 : Serial.println("On tourne a 36");
                  motA.write(36);
                  motB.write(36);
                  motC.write(36);
                  motD.write(36);
    break;
            case 52 : Serial.println("On tourne a 48");
                  motA.write(48);
                  motB.write(48);
                  motC.write(48);
                  motD.write(48);
    break;
            case 53 : Serial.println("On tourne a 60");
                  motA.write(60);
                  motB.write(60);
                  motC.write(60);
                  motD.write(60);
    break;
            case 54 : Serial.println("On tourne a 72");
                  motA.write(72);
                  motB.write(72);
                  motC.write(72);
                  motD.write(72);
    break;
            case 55 : Serial.println("On tourne a 84");
                  motA.write(84);
                  motB.write(84);
                  motC.write(84);
                  motD.write(84);
    break;
            case 56 : Serial.println("On tourne a 96");
                  motA.write(96);
                  motB.write(96);
                  motC.write(96);
                  motD.write(96);
    break;
            case 57 : Serial.println("On tourne a 108");
                  motA.write(108);
                  motB.write(108);
                  motC.write(108);
                  motD.write(108);
    break;

    
    
    }



    }    
      
  }


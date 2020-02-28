/* pin 11,5,6,10 utilisé
commence a 12 de throttle OUI
fini a 82 OUI
https://md.iutbeziers.org/FlN9Tmo9RiOoI-Lwl6zRcA#
liste des pin
Bleu(avant) droit = 10 = motD
Bleu(avant) gauche = 11 = motA
Noir(arriere) droit = 6 = motC
Noir(arriere) gauche = 5 = motB


*/

#include <Servo.h>

Servo motA,motB,motC,motD;


void writeMot(Servo motA, Servo motB, Servo motC, Servo motD, int valA, int valB, int valC, int vald) {
  motA.write(valA);
  motB.write(valB);
  motC.write(valC);
  motD.write(vald);
}



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
  motA.writeMicroseconds(1000);
  motB.writeMicroseconds(1000);
  motC.writeMicroseconds(1000);
  motD.writeMicroseconds(1000);
}
void test(){
  Serial.println("Commence dans 3...");
  delay(2000);
  Serial.println("Commence dans 2...");
  delay(2000);
  Serial.println("Commence dans 1...");
  delay(2000);
  motA.writeMicroseconds(0);
  motB.writeMicroseconds(0);
  motC.writeMicroseconds(0);
  motD.writeMicroseconds(0);
  for(int i= 1000; i< 2000; i=i+3){
    Serial.println(i);
    motA.writeMicroseconds(i);
    motB.writeMicroseconds(i);
    motC.writeMicroseconds(i);
    motD.writeMicroseconds(i);
    delay(200);
    
  }
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
                  motA.writeMicroseconds(1000);
                  motB.writeMicroseconds(1000);
                  motC.writeMicroseconds(1000);
                  motD.writeMicroseconds(1000);
    break;
        case 49 : Serial.println("On tourne a 12");
                  motA.writeMicroseconds(1860);
                  motB.writeMicroseconds(1860);
                  motC.writeMicroseconds(1860);
                  motD.writeMicroseconds(1860);
    break;
            case 50 : Serial.println("On tourne a 20");
                  motA.write(20);
                  motB.write(20);
                  motC.write(20);
                  motD.write(20);
    break;
            case 51 : Serial.println("On tourne a 28");
                  motA.write(28);
                  motB.write(28);
                  motC.write(28);
                  motD.write(28);
    break;
            case 52 : Serial.println("On tourne a 36");
                  motA.write(36);
                  motB.write(36);
                  motC.write(36);
                  motD.write(36);
    break;
            case 53 : Serial.println("On tourne a 44");
                  motA.write(44);
                  motB.write(44);
                  motC.write(44);
                  motD.write(44);
    break;
            case 54 : Serial.println("On tourne a 52");
                  motA.write(52);
                  motB.write(52);
                  motC.write(52);
                  motD.write(52);
    break;
            case 55 : Serial.println("On tourne a 60");
                  motA.write(60);
                  motB.write(60);
                  motC.write(60);
                  motD.write(60);
    break;
            case 56 : Serial.println("On tourne a 68");
                  motA.write(68);
                  motB.write(68);
                  motC.write(68);
                  motD.write(68);
    break;
            case 57 : Serial.println("On tourne a 76");
                  motA.write(76);
                  motB.write(76);
                  motC.write(76);
                  motD.write(76);

    break;
            case 97: Serial.println("TEST FCT");
                  test();
                  motA.writeMicroseconds(1000);
                  motB.writeMicroseconds(1000);
                  motC.writeMicroseconds(1000);
                  motD.writeMicroseconds(1000);

    break;


    
    
    }



    }    
      
  }
  

#include <Servo.h>
// Initialisation des moteurs, ainsi que tous les autres péripheriques en tant que variables globals, pour faciliter l'accés de celle-ci
Servo MotA; 
const int pinA;
Servo MotB;
const int pinB;
Servo MotC;
const int pinC;
Servo MotD;
const int pinD;

bool initMot(Servo *Mot, int pin){
    Mot.attach(pin, 1000,2000); 
    Mot.write(0); 
    delay(200);
    return Mot.attached()
}


void updateValue(int *a, int *b, int *c, int *d){
    a= MotA.readMicroseconds();
    b= MotB.readMicroseconds();
    c= MotC.readMicroseconds();
    d= MotD.readMicroseconds();
}
int readInputSerial(int *pitch,int *yaw,int *roll,int *throttle){
    //L'idée est d'avoir fonction qui nous renverra les bonnes valeurs, nous les traiterons autres parts
    // Juste quand nous integrerons notre reel solutions de RX nous n'aurons qu'a modifier cette fonction.
    
    


}

void setup(){
    Serial.begin(9600);// On INIT UNE COM Serie inutile en vol mais ... 
    if initMot(MotA, pinA){
        Serial.println(" MotA Right Init"); 
    }    
    else{
        Serial.println(" MotA not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }
    if initMot(MotB, pinB){
        Serial.println(" MotB Right Init");
    }
    else{    
        Serial.println(" MotA not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }   
    if initMot(MotC, pinC){
        Serial.println(" MotB Right Init");

    }
    else{
        Serial.println(" MotA not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }
    if initMot(MotD, pinD){
         Serial.println(" MotB Right Init");
    }
    else{
        Serial.println(" MotA not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }





}

void loop(){
    int actValA;
    int actValB;
    int actValC;
    int actValD;
    updateValue(&actValA, &actValB, &actValC, &actValD);
    readInput(&pitch, &yaw, &roll, &throttle);
    

}

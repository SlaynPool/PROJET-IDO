#include <Servo.h>
#include "determinant.h"

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
void updateValue(int *motValue){
    //!\ readMicroseconds renvoi des int
    motValue[0] = MotA.readMicroseconds();
    motValue[1] = MotB.readMicroseconds();
    motValue[2] = MotC.readMicroseconds();
    motValue[3] = MotD.readMicroseconds();
}
int readInputSerial(int *pitch,int *yaw,int *roll,int *throttle){
    //L'idée est d'avoir fonction qui nous renverra les bonnes valeurs, nous les traiterons autres parts
    // Juste quand nous integrerons notre reel solutions de RX nous n'aurons qu'a modifier cette fonction.
    //Par exemple, si on voudra faire passer notre RX/overIP, il nous suffira d'ouvrir un socket
}

bool setmotor(int valMotA, int valMotB, int valMotC, int valMotD){
    return TRUE;


}


bool calculMot(bool *error, float *gyroPitch, float *gyroYaw, float *gyroRoll, float *pitch, float *yaw, float *roll, float *throttle ){
    int valMotA, valMotB, valMotC, valMotD;
    float truePitch, trueYaw, trueRoll, trueThrottle; 
    //VOIR FORMULE.PDF
    //FUSION des valeurs du gyro et des Inputs:
    truePitch= pitch-gyroPitch;
    trueYaw= yaw-gyroPitch;
    trueRoll=roll-gyroRoll;
    truePitch=pitch-gyroPitch;

    
    
    





 // ON est censé avoir detrminer tous les valMot
    if setmotor(valMotA, valMotB, valMotC, valMotD){
        error=TRUE; //PAS PROBLEME
        return error;
    }else{
        error= FALSE;// PROBELEME
        return error;
    }



}
void failsafe(){ // on rentre dans un cas ou on veut que le drone soit stable, et qu'il descende lentement 
    bool again;
    while(again=TRUE){
        motVal[]= 25,25,25;
        readGyro(&addrGyro, &gyroPitch, &gyroYaw, &gyroRoll );
        calculMot(&error, &gyroPitch, &gyroYaw, &gyroRoll, &motVal );
    }


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
        Serial.println(" MotB not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }   
    if initMot(MotC, pinC){
        Serial.println(" MotC Right Init");

    }
    else{
        Serial.println(" MotC not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }
    if initMot(MotD, pinD){
         Serial.println(" MotD Right Init");
    }
    else{
        Serial.println(" MotD not fine");
        return 1; // ON STOP //SYNTAX A VERIFIER TODO 
    }
    initRX(); //TODO 
    initGyro();
}

void loop(){
    
    bool error;
    float pitch;
    float yaw;
    float roll;
    float throttle;
    float throttle[4];
    float gyroPitch;
    float gyroYaw;
    float gyroRoll;

    updateValue(&motVal);
    readInput(&pitch, &yaw, &roll, &throttle);
    readGyro(&addrGyro, &gyroPitch, &gyroYaw, &gyroRoll );
    if (calculMot(&error, &gyroPitch, &gyroYaw, &gyroRoll, &pitch, &yaw, &roll, &throttle ) == FALSE){ // on appellera le set de vitesse des moteurs à la fin de cette fonction
        failsafe(); ///// C4EST LA PANIQUE !!!!
    }
}

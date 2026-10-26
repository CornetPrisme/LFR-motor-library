#include <Arduino.h>
#include "CanMessages.h"
#include "LFR-MotorLibrary.h"

CanManager can;

MotorDriver rightMotor(&can, RIGHT);
MotorDriver leftMotor(&can, LEFT);
MotorDriver bothMotors(&can, BOTH);

void setup() {
    Serial.begin(115200);
    can.init(GPIO_NUM_11, GPIO_NUM_12);

    Serial.println("Initialisation des moteurs...");
    bothMotors.init();
    delay(8000); 
}

void loop() {
    // Avancer les deux roues à mi-vitesse
    bothMotors.set_speed(128);
    delay(2000);

    // Arrêt complet
    bothMotors.set_speed(0);
    delay(2000);

    // Tourner sur place : roue gauche en avant, roue droite en arrière
    leftMotor.set_speed(150);
    rightMotor.set_speed(-150);
    delay(1000);

    // Arrêt complet
    bothMotors.set_speed(0);
    delay(2000);

    // Arrêter uniquement la roue droite (tourner autour de la roue droite)
    rightMotor.set_speed(0);
    leftMotor.set_speed(100);
    delay(1000);

    // Arrêt complet
    bothMotors.set_speed(0);
    delay(10000);
}

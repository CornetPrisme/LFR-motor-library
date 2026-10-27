#include <Arduino.h>
#include "CanManager.h"
#include "LFR-MotorLibrary.h"

CanManager can;

MotorDriver rightMotor(&can, 0);
MotorDriver leftMotor(&can, 1);

void setup() {
    Serial.begin(115200);
    can.init(GPIO_NUM_11, GPIO_NUM_12);
    delay(2000); 
}

void loop() {
    Serial.println("Marche avant !");
    rightMotor.set_speed(150);
    leftMotor.set_speed(150);
    delay(2000);

    Serial.println("Virage à droite !");
    leftMotor.set_speed(150);
    rightMotor.set_speed(0);
    delay(2000);

    Serial.println("Arrêt !");
    rightMotor.set_speed(0);
    leftMotor.set_speed(0);
    delay(2000);

    Serial.println("Arrière toute !");
    rightMotor.set_speed(-140);
    leftMotor.set_speed(-140);
    delay(2000);

    Serial.println("Virage à gauche !");
    leftMotor.set_speed(0);
    rightMotor.set_speed(150);
    delay(2000);

    Serial.println("Arrêt !");
    rightMotor.set_speed(0);
    leftMotor.set_speed(0);
    delay(5000);
}
#include <Arduino.h>
#include "CanMessages.h"
#include "LFR-MotorLibrary.h"

CanManager can;

MotorDriver rightMotor(&can, RIGHT);
MotorDriver leftMotor(&can, LEFT);
MotorDriver bothMotors(&can, BOTH);

enum class line_pos_state_t : uint8_t {
  LOST = 0,
  DETECTED = 1,
  LOSTING = 2,
  T = 3,
  Y = 4,
  RIGHT = 5,
  FULL = 6
};

CAN_STRUCT(t_line_sensor_data, 160,
  int16_t line_pos;
  line_pos_state_t state;
);

unsigned long last_print_time = 0;

enum class MoveState { STOP, FORWARD, TURN_RIGHT, TURN_LEFT };
MoveState current_state = MoveState::STOP;

void on_line_sensor_received(can_frame_t frame) {
    t_line_sensor_data msg;
    memcpy(&msg, frame.data, frame.length);
    
    int16_t pos = msg.line_pos; 
    
    if (millis() - last_print_time > 1000) {
        Serial.print("Etat: ");
        Serial.print((int)msg.state);
        Serial.print(" | Position ligne: ");
        Serial.println(pos);
        last_print_time = millis();
    }

    MoveState next_state;

    if (pos > 1500) { 
        next_state = MoveState::TURN_RIGHT;
    } 
    else if (pos < -1500) { 
        next_state = MoveState::TURN_LEFT;
    } 
    else { 
        next_state = MoveState::FORWARD;
    }

    if (next_state != current_state) {
        current_state = next_state;

        if (current_state == MoveState::TURN_RIGHT) {
            leftMotor.set_speed(150);
            rightMotor.set_speed(0);
        } 
        else if (current_state == MoveState::TURN_LEFT) {
            leftMotor.set_speed(0);
            rightMotor.set_speed(150);
        } 
        else if (current_state == MoveState::FORWARD) {
            bothMotors.set_speed(150);
        }
    }
}

void setup() {
    Serial.begin(115200);
    can.init(GPIO_NUM_11, GPIO_NUM_12);

    Serial.println("Initialisation des moteurs...");
    bothMotors.init();
    delay(2000); 
    can.onReceive(t_line_sensor_data::ID, on_line_sensor_received);
}

void loop() {
    can.update();
}
#include <Arduino.h>
#include "CanManager.h"
#include "LFR-MotorLibrary.h"

CanManager can;

MotorDriver rightMotor(&can, 0);
MotorDriver leftMotor(&can, 1);

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
)

enum class MoveState { STOP, FORWARD, TURN_RIGHT, TURN_LEFT };
MoveState current_state = MoveState::STOP;

unsigned long last_print_time = 0;


void on_line_sensor_received(t_line_sensor_data msg) {
    int16_t pos = msg.line_pos; 
    
    if (millis() - last_print_time > 500) {
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
            leftMotor.set_speed(150);
            rightMotor.set_speed(150);
        }
    }
}


void setup() {
    can.init(GPIO_NUM_11, GPIO_NUM_12);
    delay(2000); 
    
    can.onReceive<t_line_sensor_data>(on_line_sensor_received);
}


void loop() {
    can.update();
}
#pragma once
#include "CanManager.h"

CAN_STRUCT(MOTOR, 291, 
    uint8_t motor_id;
    int16_t speed;        
)

class MotorDriver {
private:
    CanManager* _can;
    uint8_t _motor_id;
public:
    MotorDriver(CanManager* can, uint8_t motor_id) {
        _can = can;
        _motor_id = motor_id;
    }

    void set_speed(int16_t speed) {
        MOTOR msg;
        msg.motor_id = _motor_id;
        msg.speed = speed;
        _can->send(msg);
    }

    void init() {
        set_speed(0);
    }
};
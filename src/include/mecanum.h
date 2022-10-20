#pragma once

#include <Arduino.h>
#include "register.h"

class Mecanum {
    float frontLeftSpeed;
    float frontRightSpeed;
    float backLeftSpeed;
    float backRightSpeed;

    float xSpeed;
    float ySpeed;
    float zRot;
    float angle;

    float deadband;

    void clamp();
    void rotate();
    float applyDeadband(float x);
    void normalize();

    double dabs(double x); // arduino wtf

    public:
    Mecanum();
    void drive(float y, float x, float z, float gyro);
    void setDeadband(float deadband);
};

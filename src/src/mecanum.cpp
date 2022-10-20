#include "mecanum.h"

Mecanum::Mecanum() {
    analogWrite(3, 0);
    analogWrite(5, 0);
    analogWrite(6, 0);
    analogWrite(9, 0);

    deadband = .02;
    xSpeed = 0;
    ySpeed = 0;
    zRot = 0;
    angle = 0;
    frontLeftSpeed = 0;
    frontRightSpeed = 0;
    backLeftSpeed = 0;
    backRightSpeed = 0;
}

void Mecanum::clamp() {
    xSpeed = max(-1., min(xSpeed, 1.));
    ySpeed = max(-1., min(ySpeed, 1.));
    zRot = max(-1., min(zRot, 1.));
}

void Mecanum::rotate() {
    auto c = cos(angle * M_PI / 100.);
    auto s = sin(angle * M_PI / 100.);
    float tmp = xSpeed * c - ySpeed * s;
    ySpeed = xSpeed * s + ySpeed * c;
    xSpeed = tmp;
}

float Mecanum::applyDeadband(float x) {
    return dabs(x) > deadband ?
            (
                x > 0
                    ? (x - deadband) / (1. - deadband)
                    : (x + deadband) / (1. - deadband)
            ) : 0.;
}

void Mecanum::normalize() {
    // float x = dabs(frontLeftSpeed);
    // float speeds[3] = {frontRightSpeed, backLeftSpeed, backRightSpeed};
    // for (int i = 0; i < 3; i++) {
    //     x = dabs(speeds[i]) > x ? speeds
    // }
    float m = max(dabs(frontLeftSpeed), max(dabs(frontRightSpeed), max(dabs(backLeftSpeed), dabs(backRightSpeed))));
    if (m > 1.) {
        frontLeftSpeed = frontLeftSpeed / m;
        frontRightSpeed = frontRightSpeed / m;
        backLeftSpeed = backLeftSpeed / m;
        backRightSpeed = backRightSpeed / m;
    }
}

void Mecanum::drive(float y, float x, float z, float gyro) {
    xSpeed = applyDeadband(x);
    ySpeed = applyDeadband(y);
    zRot = applyDeadband(z);
    angle = gyro;
    
    clamp();
    rotate();

    frontLeftSpeed = xSpeed + ySpeed + zRot;
    frontRightSpeed = xSpeed - ySpeed - zRot;
    backLeftSpeed = xSpeed - ySpeed + zRot;
    backRightSpeed = xSpeed + ySpeed - zRot;

    normalize();

    registerWrite(0, frontLeftSpeed > 0 ? HIGH : LOW);
    registerWrite(1, frontLeftSpeed < 0 ? HIGH : LOW);
    registerWrite(2, frontRightSpeed > 0 ? HIGH : LOW);
    registerWrite(3, frontRightSpeed < 0 ? HIGH : LOW);
    registerWrite(4, backLeftSpeed > 0 ? HIGH : LOW);
    registerWrite(5, backLeftSpeed < 0 ? HIGH : LOW);
    registerWrite(6, backRightSpeed > 0 ? HIGH : LOW);
    registerWrite(7, backRightSpeed < 0 ? HIGH : LOW);

    analogWrite(3, abs(frontLeftSpeed));
    analogWrite(5, abs(frontRightSpeed));
    analogWrite(6, abs(backLeftSpeed));
    analogWrite(9, abs(backRightSpeed));

    // frontLeft.set(frontLeftSpeed);
    // frontRight.set(-frontRightSpeed);
    // backLeft.set(-backLeftSpeed);
    // backRight.set(backRightSpeed);

    Serial.println(frontLeftSpeed);
    Serial.println(frontRightSpeed);
    Serial.println(backLeftSpeed);
    Serial.println(backRightSpeed);
}

void Mecanum::setDeadband(float deadband) {
    this->deadband = deadband;
}

double Mecanum::dabs(double x) {
    return x <= 0.0 ? 0.0 - x : x;
}

#pragma once

#include <Arduino.h>
#include "QuickStepper.h"

class MotorDefinition {
public:
    MotorDefinition(uint8_t stepPin, uint8_t dirPin, uint8_t m1Pin, uint8_t m2Pin, uint8_t m3Pin);

    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t m1Pin;
    uint8_t m2Pin;
    uint8_t m3Pin;
};

class MotorBase {
public:
    MotorBase(const MotorDefinition *motor1, const MotorDefinition *motor2);
    void drive(float speed1, float speed2) const;

private:
    QuickStepper *motor1;
    QuickStepper *motor2;
};

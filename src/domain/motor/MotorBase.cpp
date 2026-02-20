#include "MotorBase.h"

MotorDefinition::MotorDefinition(
    const uint8_t stepPin, const uint8_t dirPin, const uint8_t m1Pin, const uint8_t m2Pin, const uint8_t m3Pin
) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->m1Pin = m1Pin;
    this->m2Pin = m2Pin;
    this->m3Pin = m3Pin;
}

MotorBase::MotorBase(const MotorDefinition *md1, const MotorDefinition *md2) {
    motor1 = new QuickStepper(md1->stepPin, md1->dirPin, md1->m1Pin, md1->m2Pin, md1->m3Pin, 0);
    motor1->begin();

    motor2 = new QuickStepper(md2->stepPin, md2->dirPin, md2->m1Pin, md2->m2Pin, md2->m3Pin, 1);
    motor2->begin();
}

void MotorBase::drive(const float speed1, const float speed2) const {
    motor1->setSpeed(speed1);
    motor2->setSpeed(-speed2);
}

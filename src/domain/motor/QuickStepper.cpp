#include "QuickStepper.h"

#define QS_TIMER_DIVIDER 2
#define QS_TIMER_FREQUENCY 40000000.0
#define QS_MAX_MICRO_SPEED 4000

QuickStepper *QuickStepper::instances[4] = {nullptr, nullptr, nullptr, nullptr};

QuickStepper::QuickStepper(
    const uint8_t stepPin, const uint8_t dirPin, const uint8_t m1, const uint8_t m2, const uint8_t m3,
    const uint8_t timerId
) {
    this->timerId = timerId;

    this->stepPin = stepPin;
    this->dirPin = dirPin;
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);

    this->m1 = m1;
    this->m2 = m2;
    this->m3 = m3;
    pinMode(m1, OUTPUT);
    pinMode(m2, OUTPUT);
    pinMode(m3, OUTPUT);
}

void QuickStepper::begin() {
    instances[timerId] = this;

    this->timer = timerBegin(timerId, QS_TIMER_DIVIDER, true);
    if (timerId == 0) {
        timerAttachInterrupt(timer, &QuickStepper::isr_wrapper0, true);
    } else if (timerId == 1) {
        timerAttachInterrupt(timer, &QuickStepper::isr_wrapper1, true);
    }

    this->enabled = false;
}

void QuickStepper::setSpeed(const float speed) {
    updateDirection(speed);
    updateMicroSpeed(speed);
    this->speed = speed;
}

float QuickStepper::getSpeed() const {
    return speed;
}

uint32_t QuickStepper::getSteps() {
    return steps;
}

void QuickStepper::updateDirection(const float speed) {
    const uint8_t newDirection = speed > 0 ? 1 : -1;
    if (newDirection == direction) {
        return;
    }

    direction = newDirection;
    digitalWrite(dirPin, direction == 1 ? HIGH : LOW);
}

void QuickStepper::updateMicroSpeed(const float speed) {
    const float absSpeed = abs(speed);

    int newMicroSteps;
    if (absSpeed < 200) {
        newMicroSteps = 16;
        digitalWrite(m1, HIGH);
        digitalWrite(m2, HIGH);
        digitalWrite(m3, HIGH);
    } else if (absSpeed < 400) {
        newMicroSteps = 8;
        digitalWrite(m1, HIGH);
        digitalWrite(m2, HIGH);
        digitalWrite(m3, LOW);
    } else if (absSpeed < 800) {
        newMicroSteps = 4;
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        digitalWrite(m3, LOW);
    } else if (absSpeed < 1600) {
        newMicroSteps = 2;
        digitalWrite(m1, HIGH);
        digitalWrite(m2, LOW);
        digitalWrite(m3, LOW);
    } else {
        newMicroSteps = 1;
        digitalWrite(m1, LOW);
        digitalWrite(m2, LOW);
        digitalWrite(m3, LOW);
    }
    microSteps = newMicroSteps;

    const float newMicroSpeed = constrain(absSpeed * microSteps, 0, QS_MAX_MICRO_SPEED);
    if (newMicroSpeed != microSpeed) {
        if (newMicroSpeed < 0.5) {
            timerAlarmWrite(timer, QS_TIMER_FREQUENCY, true);
            timerAlarmDisable(timer);
            enabled = false;
        } else {
            timerAlarmWrite(timer, QS_TIMER_FREQUENCY / newMicroSpeed, true);
            if (!enabled) {
                timerAlarmEnable(timer);
                enabled = true;
            }
        }

        microSpeed = newMicroSpeed;
    }
}

void IRAM_ATTR QuickStepper::isr_wrapper0() {
    instances[0]->step();
}

void IRAM_ATTR QuickStepper::isr_wrapper1() {
    instances[1]->step();
}

void IRAM_ATTR QuickStepper::isr_wrapper2() {
    instances[2]->step();
}

void IRAM_ATTR QuickStepper::isr_wrapper3() {
    instances[3]->step();
}

void QuickStepper::step() {
    GPIO.out_w1ts = 1 << stepPin;
    volatile uint32_t i = 0;
    for(i=0; i<50; i++) { __asm__ __volatile__("nop"); }
    GPIO.out_w1tc = 1 << stepPin;

    steps += direction;
}

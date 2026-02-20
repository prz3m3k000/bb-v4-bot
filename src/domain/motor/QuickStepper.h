#pragma once

#include <Arduino.h>

class QuickStepper {
public:
    QuickStepper(uint8_t stepPin, uint8_t dirPin, uint8_t m1, uint8_t m2, uint8_t m3, uint8_t timerId);

    void begin();

    void setSpeed(float speed);

    float getSpeed() const;

    uint32_t getSteps();

private:
    uint8_t timerId;
    uint8_t stepPin;
    uint8_t dirPin;
    uint8_t m1;
    uint8_t m2;
    uint8_t m3;

    hw_timer_t *timer;

    bool enabled;
    int8_t direction;
    uint8_t microSteps = 1;
    float microSpeed = 0;
    float speed = 0;

    volatile bool pinState = 0;
    volatile uint32_t steps = 0;


    // ---
    static QuickStepper *instances[4];

    static void IRAM_ATTR isr_wrapper0();

    static void IRAM_ATTR isr_wrapper1();

    static void IRAM_ATTR isr_wrapper2();

    static void IRAM_ATTR isr_wrapper3();

    // ---

    void updateDirection(float speed);

    void updateMicroSpeed(float speed);

    void IRAM_ATTR step();
};

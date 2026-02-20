#include "Bot.h"

#include "motion_processing/MotionProcessing.h"

#define SPEED_LOOP_INTERVAL_MS 10
#define PITCH_LOOP_INTERVAL_MS 1
#define LOG_LOOP_INTERVAL_MS 100

Bot::Bot(PidController *speedPid, PidController *pitchPid, MotorBase *motorBase, MotionProcessing *motionProcessing) {
    this->speedPid = speedPid;
    this->pitchPid = pitchPid;
    this->motorBase = motorBase;
    this->motionProcessing = motionProcessing;
    this->timer = new StopWatch();
    this->speedTimer = new StopWatch();
    this->pitchTimer = new StopWatch();
    this->logTimer = new StopWatch();
}

void Bot::nextStep() {
    const float dt = timer->delta() * 1e-3f;

    float pitch = 0.0f;
    float roll = 0.0f;
    float yaw = 0.0f;
    float gyroRateX = 0.0f;
    float gyroRateY = 0.0f;
    float gyroRateZ = 0.0f;
    motionProcessing->computeOrientation(&pitch, &roll, &yaw, &gyroRateX, &gyroRateY, &gyroRateZ);

    if (speedTimer->elapsed() >= SPEED_LOOP_INTERVAL_MS) {
        const float dt = speedTimer->elapsed() * 1e-3f;
        pitchSetpoint = speedPid->compute(speed, speedSetpointOffset, dt);
        speedTimer->reset();
    }


    if (pitchTimer->elapsed() >= PITCH_LOOP_INTERVAL_MS) {
        const float dt = pitchTimer->elapsed() * 1e-3f;

        const float pitchFeedback = pitchPid->compute2(pitch, gyroRateX , pitchSetpoint - pitchSetpointOffset, dt);
        speed = -pitchFeedback;
        motorBase->drive(speed, speed);

        if (logTimer->elapsed() >= LOG_LOOP_INTERVAL_MS) {
            Serial.printf("S: %6.2f (OFF: %6.2f), P: %6.2f (OFF: %6.2f), R: %6.2f, Y: %6.2f | GXR: %6.2f, GYR: %6.2f, GZR: %6.2f | SF: %6.2f, PR: %6.2f\n",speed, speedSetpointOffset, pitch, pitchSetpointOffset, roll, yaw, gyroRateX, gyroRateY, gyroRateZ, pitchSetpoint, pitchFeedback);
            logTimer->reset();
        }

        pitchTimer->reset();
    }
}

void Bot::setDashboardAddress(IPAddress ip, uint16_t port) {
    // noop
}

void Bot::setSpeedPidCoefficients(const float p, const float i, const float d, const float output) const {
    this->speedPid->setCoefficients(p, i, d);
    this->speedPid->setOutputRange(-output, output);
}

void Bot::setPitchPidCoefficients(const float p, const float i, const float d, const float output) const {
    this->pitchPid->setCoefficients(p, i, d);
    this->pitchPid->setOutputRange(-output, output);
}

void Bot::setSetpointOffsets(const float speedSetpointOffset, const float pitchSetpointOffset) {
    this->speedSetpointOffset = speedSetpointOffset;
    this->pitchSetpointOffset = pitchSetpointOffset;
}

void Bot::resetSpeedPid() const {
    // this->speedPid->reset();
}

void Bot::resetPitchPid() const {
    // thos->pitchPid->reset();
}
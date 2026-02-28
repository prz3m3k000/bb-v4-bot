#include "Bot.h"

#include "filters/Butterworth.h"
#include "motion_processing/MotionProcessing.h"

#define SPEED_LOOP_INTERVAL_MS 2
#define PITCH_LOOP_INTERVAL_MS 1
#define LOG_LOOP_INTERVAL_MS 20

Bot::Bot(PidController *speedPid, PidController *pitchPid, MotorBase *motorBase, MotionProcessing *motionProcessing, OutboundCommandSender *commandSender) {
    this->speedPid = speedPid;
    this->pitchPid = pitchPid;
    this->motorBase = motorBase;
    this->motionProcessing = motionProcessing;
    this->commandSender = commandSender;
    this->timer = new StopWatch();
    this->speedTimer = new StopWatch();
    this->pitchTimer = new StopWatch();
    this->logTimer = new StopWatch();
    this->pitchFeedbackFilter = new Butterworth(0.02008336556421122521, -0.64135153805756306422, 1.56101807580071816339);
}

void Bot::nextStep() {
    if (speedTimer->elapsed() >= SPEED_LOOP_INTERVAL_MS) {
        const float dt = speedTimer->elapsed() * 1e-3f;
        pitchSetpoint = -speedPid->compute(speed, speedSetpointOffset, dt);
        speedTimer->reset();
    }

    if (pitchTimer->elapsed() >= PITCH_LOOP_INTERVAL_MS) {
        const float dt = pitchTimer->elapsed() * 1e-3f;
        float pitch = 0.0f;
        float roll = 0.0f;
        float yaw = 0.0f;
        float ax, ay, az;
        float gx, gy, gz;
        motionProcessing->computeOrientation(dt, &pitch, &roll, &yaw, &ax, &ay, &az, &gx, &gy, &gz);

        const float pitchFeedback = pitchPid->compute(pitch, pitchSetpoint - pitchSetpointOffset, dt);
        speed = pitchFeedbackFilter->filter(pitchFeedback);
        motorBase->drive(speed, speed);

        if (logTimer->elapsed() >= LOG_LOOP_INTERVAL_MS) {
            // Serial.printf("S: %6.2f (OFF: %6.2f), P: %6.2f (OFF: %6.2f), R: %6.2f, Y: %6.2f | GXR: %6.2f, GYR: %6.2f, GZR: %6.2f | SF: %6.2f, PR: %6.2f\n",speed, speedSetpointOffset, pitch, pitchSetpointOffset, roll, yaw, gyroRateX, gyroRateY, gyroRateZ, pitchSetpoint, pitchFeedback);
            // Serial.printf("dt: %6.4f, S: %6.2f, P: %6.2f, A: (%6.2f, %6.2f, %6.2f), G: (%6.2f, %6.2f, %6.2f) \n", dt, speed, pitch, ax, ay, az, gx, gy, gz);
            sendBotTelemetry(0, dt, ax, ay, az, gx, gy, gz, pitch, roll, yaw);
            logTimer->reset();
        }

        pitchTimer->reset();
    }
}
void Bot::setDashboardAddress(const IPAddress &remoteIP, const uint16_t remotePort) const {
    Serial.printf("setDashboardAddress called: %s, %d, %p\n", remoteIP.toString().c_str(), remotePort, commandSender);
    this->commandSender->setRemote(remoteIP, remotePort);
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
    // this->pitchPid->reset();
}

void Bot::sendBotTelemetry(float timestamp, float dt, float ax, float ay, float az, float gx, float gy, float gz, float pitch, float roll, float yaw) const {
    commandSender->sendBotTelemetry(timestamp, dt, ax, ay, az, gx, gy, gz, pitch, roll, yaw);
}

void Bot::sendPitchPidTelemetry() const {
    commandSender->sendPitchPidTelemetry(pitchPid);
}

void Bot::sendSpeedPidTelemetry() const {
    commandSender->sendPitchPidTelemetry(speedPid);
}
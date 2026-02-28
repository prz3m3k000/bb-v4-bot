#pragma once

#include <Arduino.h>

#include "filters/Butterworth.h"
#include "infra/OutboundCommandSender.h"
#include "motion_processing/MotionProcessing.h"
#include "motor/MotorBase.h"
#include "pid/PidController.h"
#include "stop_watch/StopWatch.h"

class Bot {
public:
    Bot(PidController *speedPid, PidController *pitchPid, MotorBase *motorBase, MotionProcessing *motionProcessing, OutboundCommandSender *commandSender);

    void setSpeedPidCoefficients(float p, float i, float d, float output) const;

    void setSpeedPidOutput(float output) const;

    void setPitchPidOutput(float output) const;

    void setPitchPidCoefficients(float p, float i, float d, float output) const;

    void setSetpointOffsets(float speedSetpointOffset, float pitchSetpointsOffset);

    void resetSpeedPid() const;

    void resetPitchPid() const;

    void sendBotTelemetry(float timestamp, float dt, float ax, float ay, float az, float gx, float gy, float gz, float pitch, float roll, float yaw) const;

    void sendPitchPidTelemetry() const;

    void sendSpeedPidTelemetry() const;

    void nextStep();

    void setDashboardAddress(const IPAddress &remoteIP, uint16_t remotePort) const;

private:
    PidController *speedPid;
    PidController *pitchPid;
    MotorBase *motorBase;
    MotionProcessing *motionProcessing;
    OutboundCommandSender *commandSender;

    StopWatch *timer;
    StopWatch *speedTimer;
    StopWatch *pitchTimer;
    StopWatch *logTimer;
    Butterworth *pitchFeedbackFilter;

    float pitchSetpoint = 0;
    float speedSetpointOffset = 0;
    float pitchSetpointOffset = 0;
    float speed = 0;
};

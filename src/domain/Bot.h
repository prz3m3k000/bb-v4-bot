#pragma once

#include <Arduino.h>

#include "motion_processing/MotionProcessing.h"
#include "motor/MotorBase.h"
#include "pid/PidController.h"
#include "stop_watch/StopWatch.h"

class Bot {
public:
    Bot(PidController *speedPid, PidController *pitchPid, MotorBase *motorBase, MotionProcessing *motionProcessing);

    void setDashboardAddress(IPAddress ip, uint16_t port);

    void setSpeedPidCoefficients(float p, float i, float d, float output) const;

    void setSpeedPidOutput(float output) const;

    void setPitchPidOutput(float output) const;

    void setPitchPidCoefficients(float p, float i, float d, float output) const;

    void setSetpointOffsets(float speedSetpointOffset, float pitchSetpointsOffset);

    void resetSpeedPid() const;

    void resetPitchPid() const;

    void nextStep();

private:
    PidController *speedPid;
    PidController *pitchPid;
    MotorBase *motorBase;
    MotionProcessing *motionProcessing;

    StopWatch *timer;
    StopWatch *speedTimer;
    StopWatch *pitchTimer;
    StopWatch *logTimer;

    float pitchSetpoint = 0;
    float speedSetpointOffset = 0;
    float pitchSetpointOffset = 0;
    float speed = 0;
};

#pragma once

// #include <MPU6050_6Axis_MotionApps20.h>
#include "MPU6050.h"

class MotionProcessing {
public:
    MotionProcessing(float alpha);
    void computeOrientation(float dt, float *pitch, float *roll, float *yaw, float *ax, float *ay, float *az, float *gx, float *gy, float *gz);

    void calibrate() const;

    void restoreCalibration() const;

private:
    MPU6050* imu;
    float alpha;
    float lastPitch = 0;
    float lastRoll = 0;
    float lastYaw = 0;
    float lastAccelY = 0;
    float lastGyroRateX = 0;
    float lastGyroRateY = 0;
    float lastGyroRateZ = 0;
};

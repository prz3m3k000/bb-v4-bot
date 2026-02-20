#pragma once

#include <MPU6050_6Axis_MotionApps20.h>

class MotionProcessing {
public:
    MotionProcessing(float alpha);
    void computeOrientation(float *pitch, float *roll, float *yaw, float *gyroRateX, float *gyroRateY, float *gyroRateZ);

    void calibrate() const;

    void restoreCalibration() const;

private:
    MPU6050* imu;
    float alpha;
    float lastPitch = 0;
    float lastRoll = 0;
    float lastYaw = 0;
    float lastGyroRateX = 0;
    float lastGyroRateY = 0;
    float lastGyroRateZ = 0;
};

#include "MotionProcessing.h"

MotionProcessing::MotionProcessing(float alpha) {
    this->imu = new MPU6050();
    imu->initialize();
    imu->dmpInitialize();

    // calibrate();
    restoreCalibration();

    imu->setDMPEnabled(true);
    this->alpha = alpha;
}

void MotionProcessing::computeOrientation(float *pitch, float *roll, float *yaw, float *gyroRateX, float *gyroRateY, float *gyroRateZ) {
    uint8_t fifoBuffer[64];
    if (imu->dmpGetCurrentFIFOPacket(fifoBuffer)) {
        Quaternion q;
        imu->dmpGetQuaternion(&q, fifoBuffer);

        VectorFloat gravity;
        imu->dmpGetGravity(&gravity, &q);

        float ypr[3];
        imu->dmpGetYawPitchRoll(ypr, &q, &gravity);

        VectorInt16 gyro;
        imu->dmpGetGyro(&gyro, fifoBuffer);

        lastPitch = ypr[2] * 180.0 / M_PI;
        lastRoll = ypr[1] * 180.0 / M_PI;
        lastYaw = ypr[0] * 180.0 / M_PI;
        lastGyroRateX = gyro.x;
        lastGyroRateY = gyro.y;
        lastGyroRateZ = gyro.z;
    }

    *pitch = lastPitch;
    *roll = lastRoll;
    *yaw = lastYaw;

    *gyroRateX = lastGyroRateX;
    *gyroRateY = lastGyroRateY;
    *gyroRateZ = lastGyroRateZ;
}

void MotionProcessing::calibrate() const {
    Serial.println("IMU: calibration");

    imu->setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    imu->setFullScaleAccelRange(MPU6050_ACCEL_FS_2);

    imu->setXAccelOffset(0);
    imu->setYAccelOffset(0);
    imu->setZAccelOffset(0);
    imu->setXGyroOffset(0);
    imu->setYGyroOffset(0);
    imu->setZGyroOffset(0);

    imu->CalibrateAccel(6);
    imu->CalibrateGyro(6);
    imu->PrintActiveOffsets();

    const int16_t ax_off = imu->getXAccelOffset();
    const int16_t ay_off = imu->getYAccelOffset();
    const int16_t az_off = imu->getZAccelOffset();
    const int16_t gx_off = imu->getXGyroOffset();
    const int16_t gy_off = imu->getYGyroOffset();
    const int16_t gz_off = imu->getZGyroOffset();
    Serial.printf("IMU: calibration complete\n");
    Serial.printf("\t- ax = %d\n", ax_off);
    Serial.printf("\t- ay = %d\n", ay_off);
    Serial.printf("\t- az = %d\n", az_off);
    Serial.printf("\t- gx = %d\n", gx_off);
    Serial.printf("\t- gy = %d\n", gy_off);
    Serial.printf("\t- gz = %d\n", gz_off);
}

void MotionProcessing::restoreCalibration() const {
    Serial.println("IMU: Restoring calibration");

    imu->setXAccelOffset(-1390);
    imu->setYAccelOffset(-4898);
    imu->setZAccelOffset(1622);
    imu->setXGyroOffset(183);
    imu->setYGyroOffset(-26);
    imu->setZGyroOffset(-6);
}

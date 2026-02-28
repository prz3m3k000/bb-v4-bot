#include "MotionProcessing.h"

MotionProcessing::MotionProcessing(float alpha) {
    this->imu = new MPU6050();
    imu->initialize();
    imu->setFullScaleGyroRange(MPU6050_GYRO_FS_250);
    imu->setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
    imu->setDLPFMode(MPU6050_DLPF_BW_42);


    restoreCalibration();

    imu->setDMPEnabled(true);
    this->alpha = alpha;
}

void MotionProcessing::computeOrientation(float dt, float *pitch, float *roll, float *yaw, float *ax, float *ay, float *az, float *gx, float *gy, float *gz) {
    int16_t iax, iay, iaz;
    int16_t igx, igy, igz;
    imu->getMotion6(&iax, &iay, &iaz, &igx, &igy, &igz);

    float accelX = iax / 16384.0f;
    float accelY = iay / 16384.0f;
    float accelZ = iaz / 16384.0f;
    float gyroX = igx / 131.0f;
    float gyroY = igy / 131.0f;
    float gyroZ = igz / 131.0f;

    lastAccelY = 0.95f * lastAccelY + 0.05f * accelY;
    float accelAngel = atan2(lastAccelY, accelZ) * 180.0f / M_PI;
    lastPitch = alpha * (lastPitch + gyroX * dt) + (1 - alpha) * accelAngel;

    *pitch = lastPitch;
    *roll = 0;
    *yaw = 0;

    *ax = accelX;
    *ay = lastAccelY;
    *az = accelZ;

    *gx = gyroX;
    *gy = gyroY;
    *gz = gyroZ;
}

void computeOrientation(float *pitch, float *roll, float *yaw, float *gyroRateX, float *gyroRateY, float *gyroRateZ) {

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

#include "m5.h"


void m5_init() {
    auto cfg = M5.config();
    cfg.serial_baudrate = 115200;
    cfg.clear_display = true;

    M5.begin(cfg);

    Serial.println("M5Unified started");

    if (!M5.Imu.isEnabled()) {
        Serial.println("IMU not available");
        while (true) delay(100);
    }
}
#pragma once

#include <Arduino.h>

enum class DashboardCommandId: uint8_t {
    DB_BOT_TELEMETRY = 1,
    DB_SPEED_PID_TELEMETRY = 2,
    DB_PITCH_PID_TELEMETRY = 3,
    Count
};

#pragma pack(push, 1)
struct BotTelemetryMsg {
    DashboardCommandId cmd;
    uint32_t timestamp;
    uint32_t dt;
    float ax, ay, az;
    float pitch, roll, yaw;
};

struct BotPidTelemetryMsg {
    DashboardCommandId cmd;
    uint32_t timestamp, dt;
    float setpoint, measurement, output;
    float p_term, i_term, d_term;
};
#pragma pack(pop)

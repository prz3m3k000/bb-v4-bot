#pragma once

#include <Arduino.h>

enum class InboundCommandId: uint8_t {
    BOT_SPEED_PID_COEFFICIENTS = 1,
    BOT_PITCH_PID_COEFFICIENTS = 2,
    BOT_MOTORBASE_ENABLE = 3,
    BOT_MOTORBASE_DISABLE = 4,
    BOT_DASHBOARD_ADDRESS = 5,
    Count
};

enum class DashboardCommandId: uint8_t {
    DB_BOT_TELEMETRY = 1,
    DB_SPEED_PID_TELEMETRY = 2,
    DB_PITCH_PID_TELEMETRY = 3,
    Count
};

#pragma pack(push, 1)
struct PidCoefficientsMsg {
    uint8_t cmd;
    float p;
    float i;
    float d;
};

struct DashboardAddressMsg {
    uint8_t cmd;
    uint16_t port;
};

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

#pragma once

#include <Arduino.h>

enum class InboundCommandId: uint8_t {
    BOT_SPEED_PID_COEFFICIENTS = 1,
    BOT_PITCH_PID_COEFFICIENTS = 2,
    BOT_MOTORBASE_ENABLE = 3,
    BOT_MOTORBASE_DISABLE = 4,
    BOT_DASHBOARD_ADDRESS = 5,
    BOT_SPEED_PID_RESET = 6,
    BOT_PITCH_PID_RESET = 7,
    BOT_SETPOINTS = 8,
    Count
};

#pragma pack(push, 1)
struct PidCoefficientsMsg {
    uint8_t cmd;
    float p;
    float i;
    float d;
    float output;
};

struct SetpointsMsg {
    uint8_t cmd;
    float speed;
    float pitch;
};

struct DashboardAddressMsg {
    uint8_t cmd;
    uint16_t port;
};
#pragma pack(pop)

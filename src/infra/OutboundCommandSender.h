#pragma once
#include <WiFiUdp.h>

#include "domain/pid/PidController.h"


class OutboundCommandSender {

public:
    OutboundCommandSender(WiFiUDP *udp);

    void setRemote(const IPAddress &remoteIP, uint16_t remotePort);

    void sendBotTelemetry(
        float timestamp, float dt,
        float ax, float ay, float az,
        float gx, float gy, float gz,
        float pitch, float roll, float yaw) const;

    void sendPitchPidTelemetry(const PidController *pid) const;

    void sendSpeedPidTelemetry(const PidController *pid) const;

private:
    WiFiUDP *udp = nullptr;
    IPAddress remoteIP;
    uint16_t remotePort = 0;

    void send(const uint8_t *data, uint16_t len) const;

};

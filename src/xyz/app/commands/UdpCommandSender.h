#pragma once
#include <WiFiUdp.h>
#include "Commands.h"

class UdpCommandSender {
public:
    UdpCommandSender(WiFiUDP *udp);

    void setRemoteAddress(const IPAddress &ip, const uint16_t port);

    void sendBotTelemetry(
        uint32_t timestamp,
        uint32_t dt,
        float ax,
        float ay,
        float az,
        float pitch,
        float roll,
        float yaw
    ) const;

    void sendSpeedPidTelemetry(
        uint32_t timestamp,
        uint32_t dt,
        float measurement,
        float setpoint,
        float p_term,
        float i_term,
        float d_term,
        float output
    ) const;

    void sendPitchPidTelemetry(
        uint32_t timestamp,
        uint32_t dt,
        float measurement,
        float setpoint,
        float p_term,
        float i_term,
        float d_term,
        float output
    ) const;

private:
    WiFiUDP *udp;
    IPAddress remoteAddress;
    uint16_t remotePort;

    void sendPidTelemetry(DashboardCommandId cmdId) const;

    void sendData(const uint8_t *data, uint16_t len) const;
};

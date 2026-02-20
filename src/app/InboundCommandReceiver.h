#pragma once

#include <WiFiUdp.h>
#include "domain/Bot.h"

class InboundCommandReceiver {
public:
    InboundCommandReceiver(Bot *bot, WiFiUDP *udp);

    void receiveAndHandle() const;

private:
    Bot *bot;
    WiFiUDP *udp;

    uint16_t receive(uint8_t *data, uint16_t len) const;

    void handle(const uint8_t *data, uint16_t len) const;

    void handleSpeedPidCoefficients(const uint8_t *data, uint16_t len) const;

    void handlePitchPidCoefficients(const uint8_t *data, uint16_t len) const;

    void handleDashboardAddress(const uint8_t *data, uint16_t len, const IPAddress &ip) const;

    void handleSpeedPidReset() const;

    void handlePitchPidReset() const;

    void handleSetpoints(const uint8_t *data, uint16_t len) const;

    void handleDashboardAddress() const;
};

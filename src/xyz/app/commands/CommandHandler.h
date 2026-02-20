#pragma once

#include <Arduino.h>

#include "UdpCommandSender.h"

class CommandHandler {
public:
    virtual ~CommandHandler() = default;

    virtual void handleCommand(uint8_t *data, uint16_t len, IPAddress ip) = 0;
};

class PidCoefficientsHandler : public CommandHandler {
public:
    void handleCommand(uint8_t *data, uint16_t len, IPAddress ip) override;
};

class DashboardAddressHandler : public CommandHandler {
public:
    DashboardAddressHandler(UdpCommandSender *commandSender);

    void handleCommand(uint8_t *data, uint16_t len, IPAddress ip) override;

private:
    UdpCommandSender *commandSender;
};

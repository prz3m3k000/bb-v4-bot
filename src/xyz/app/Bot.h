#pragma once

#include <WiFiUdp.h>

#include "commands/UdpCommandRouter.h"
#include "commands/UdpCommandSender.h"

class Bot {
public:
    explicit Bot(WiFiUDP *udp);
    void receiveAndHandleCommand() const;
    void step();

private:
    WiFiUDP *udp;
    UdpCommandRouter *commandRouter;
    UdpCommandSender * commandSender;
};

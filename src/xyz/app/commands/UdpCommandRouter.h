#pragma once

#include <WiFiUdp.h>
#include "CommandHandler.h"
#include "Commands.h"


class UdpCommandRouter {
public:
    UdpCommandRouter(WiFiUDP *udp);

    void receiveAndHandle() const;

    void addHandler(InboundCommandId id, CommandHandler *handler);

private:
    WiFiUDP *udp;
    CommandHandler *commandHandlers[50] = {};
};

#include "UdpCommandRouter.h"

#include "Commands.h"

UdpCommandRouter::UdpCommandRouter(WiFiUDP *udp) {
    this->udp = udp;
}

void UdpCommandRouter::receiveAndHandle() const {
    const uint16_t packetSize = udp->parsePacket();
    if (!packetSize) {
        return;
    }

    uint8_t buffer[64];
    if (packetSize > sizeof(buffer)) {
        udp->flush();
        return;
    }

    udp->read(buffer, packetSize);
    const uint8_t commandId = buffer[0];

    CommandHandler *handler = commandHandlers[commandId];
    if (!handler) {
        Serial.printf("No handler found for command id = %d\n", commandId);
        return;
    }

    handler->handleCommand(buffer, packetSize, udp->remoteIP());
}

void UdpCommandRouter::addHandler(InboundCommandId id, CommandHandler *handler) {
    const size_t idx = static_cast<size_t>(id);
    commandHandlers[idx] = handler;
}

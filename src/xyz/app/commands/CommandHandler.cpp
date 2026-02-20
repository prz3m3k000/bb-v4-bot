#include "Commands.h"
#include "CommandHandler.h"

void PidCoefficientsHandler::handleCommand(uint8_t *data, const uint16_t len, IPAddress ip) {
    if (len != sizeof(PidCoefficientsMsg)) {
        Serial.printf("Invalid PidCoefficientsMsg packet size. Got %d, but expected %d", len, sizeof(PidCoefficientsMsg));
        return;
    }

    PidCoefficientsMsg msg;
    memcpy(&msg, data, sizeof(msg));

    Serial.printf("PidCoefficientsMsg = { p: %.3f, i: %.3f, d: %.3f }\n", msg.p, msg.i, msg.d);
}

DashboardAddressHandler::DashboardAddressHandler(UdpCommandSender *commandSender) {
    this->commandSender = commandSender;
}

void DashboardAddressHandler::handleCommand(uint8_t *data, const uint16_t len, IPAddress ip) {
    if (len != sizeof(DashboardAddressMsg)) {
        Serial.printf("Invalid DashboardAddressMsg packet size. Got %d, but expected %d", len, sizeof(DashboardAddressMsg));
        return;
    }

    DashboardAddressMsg msg;
    memcpy(&msg, data, sizeof(msg));

    Serial.printf("DashboardAddressMsg = { id: %s, port: %d }\n", ip.toString().c_str(), msg.port);
    commandSender->setRemoteAddress(ip, msg.port);
}

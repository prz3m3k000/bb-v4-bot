#include "InboundCommandReceiver.h"
#include "InboundCommands.h"

InboundCommandReceiver::InboundCommandReceiver(Bot *bot, WiFiUDP *udp) {
    this->bot = bot;
    this->udp = udp;
}

void InboundCommandReceiver::receiveAndHandle() const {
    uint8_t data[64];
    const int len = this->receive(data, sizeof(data));
    if (len <= 0) {
        return;
    }

    this->handle(data, len);
}

uint16_t InboundCommandReceiver::receive(uint8_t *data, const uint16_t len) const {
    const uint16_t packetSize = udp->parsePacket();
    if (!packetSize) {
        return 0;
    }

    if (packetSize > len) {
        Serial.printf("InboundCommandReceiver: packet size (%d)\n", packetSize);
        udp->flush();
        return 0;
    }

    return udp->read(data, packetSize);
}

void InboundCommandReceiver::handle(const uint8_t *data, const uint16_t len) const {
    const auto commandId = static_cast<InboundCommandId>(data[0]);

    if (commandId == InboundCommandId::BOT_SPEED_PID_COEFFICIENTS) {
        handleSpeedPidCoefficients(data, len);
    } else if (commandId == InboundCommandId::BOT_PITCH_PID_COEFFICIENTS) {
        handlePitchPidCoefficients(data, len);
    } else if (commandId == InboundCommandId::BOT_MOTORBASE_ENABLE) {
        //
    } else if (commandId == InboundCommandId::BOT_MOTORBASE_DISABLE) {
        //
    } else if (commandId == InboundCommandId::BOT_DASHBOARD_ADDRESS) {
        handleDashboardAddress(data, len, udp->remoteIP());
    } else if (commandId == InboundCommandId::BOT_SPEED_PID_RESET) {
        handleSpeedPidReset();
    } else if (commandId == InboundCommandId::BOT_PITCH_PID_RESET) {
        handleSpeedPidReset();
    } else if (commandId == InboundCommandId::BOT_SETPOINTS) {
        handleSetpoints(data, len);
    }  else {
        Serial.printf("Unknown InboundCommandId id: %d\n", commandId);
    }
}

void InboundCommandReceiver::handleSpeedPidCoefficients(const uint8_t *data, const uint16_t len) const {
    if (len != sizeof(PidCoefficientsMsg)) {
        Serial.printf("Invalid PidCoefficientsMsg packet size (%d)", len);
        return;
    }

    PidCoefficientsMsg msg{};
    memcpy(&msg, data, sizeof(msg));

    Serial.printf("[Speed] PidCoefficientsMsg = { p: %.3f, i: %.3f, d: %.3f, output: %.3f }\n", msg.p, msg.i, msg.d, msg.output);
    bot->setSpeedPidCoefficients(msg.p, msg.i, msg.d, msg.output);
}

void InboundCommandReceiver::handlePitchPidCoefficients(const uint8_t *data, const uint16_t len) const {
    if (len != sizeof(PidCoefficientsMsg)) {
        Serial.printf("Invalid PidCoefficientsMsg packet size (%d)", len);
        return;
    }

    PidCoefficientsMsg msg{};
    memcpy(&msg, data, sizeof(msg));

    Serial.printf("[Pitch] PidCoefficientsMsg = { p: %.3f, i: %.3f, d: %.3f, output: %.3f }\n", msg.p, msg.i, msg.d, msg.output);
    bot->setPitchPidCoefficients(msg.p, msg.i, msg.d, msg.output);
}

void InboundCommandReceiver::handleDashboardAddress(const uint8_t *data, const uint16_t len, const IPAddress &ip) const {
    if (len != sizeof(DashboardAddressMsg)) {
        Serial.printf("Invalid DashboardAddressMsg packet size (%d)", len);
        return;
    }

    DashboardAddressMsg msg{};
    memcpy(&msg, data, sizeof(msg));

    bot->setDashboardAddress(ip, msg.port);
}

void InboundCommandReceiver::handleSpeedPidReset() const {
    bot->resetSpeedPid();
}

void InboundCommandReceiver::handlePitchPidReset() const {
    bot->resetPitchPid();
}

void InboundCommandReceiver::handleSetpoints(const uint8_t *data, const uint16_t len) const {
    if (len != sizeof(SetpointsMsg)) {
        Serial.printf("Invalid SetpointsMsg packet size (%d)", len);
        return;
    }

    SetpointsMsg msg{};
    memcpy(&msg, data, sizeof(msg));

    Serial.printf("SetpointsMsg = { speed: %.3f, pitch: %.3f }\n", msg.speed, msg.pitch);
    bot->setSetpointOffsets(msg.speed, msg.pitch);
}

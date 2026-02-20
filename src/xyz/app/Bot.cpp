#include "Bot.h"

#include "M5Unified.hpp"
#include "commands/UdpCommandRouter.h"
#include "commands/UdpCommandSender.h"

Bot::Bot(WiFiUDP *udp) {
    this->udp = udp;

    this->commandSender = new UdpCommandSender(udp);

    this->commandRouter = new UdpCommandRouter(udp);
    this->commandRouter->addHandler(InboundCommandId::BOT_PITCH_PID_COEFFICIENTS, new PidCoefficientsHandler());
    this->commandRouter->addHandler(InboundCommandId::BOT_SPEED_PID_COEFFICIENTS, new PidCoefficientsHandler());
    this->commandRouter->addHandler(InboundCommandId::BOT_DASHBOARD_ADDRESS, new DashboardAddressHandler(commandSender));
}

void Bot::receiveAndHandleCommand() const {
    commandRouter->receiveAndHandle();
}

void Bot::step() {
    float ax, ay, az;
    // float pitch, roll, yaw;
    M5.update();
    M5.Imu.getAccel(&ax, &ay, &az);

    uint32_t timestamp = micros();
    uint32_t dt = 10;

    commandRouter->receiveAndHandle();
    commandSender->sendBotTelemetry(timestamp, dt, ax, ay, az, 0, 0, 0);
}


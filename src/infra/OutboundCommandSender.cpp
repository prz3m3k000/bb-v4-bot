//
// Created by prz3m on 22.02.2026.
//

#include "OutboundCommandSender.h"
#include "OutboundCommands.h"
#include "domain/pid/PidController.h"

OutboundCommandSender::OutboundCommandSender(WiFiUDP *udp) {
    this->udp = udp;
}

void OutboundCommandSender::setRemote(const IPAddress &remoteIP, const uint16_t remotePort) {
    this->remoteIP = remoteIP;
    this->remotePort = remotePort;
}

void OutboundCommandSender::sendBotTelemetry(
    const float timestamp, const float dt,
    const float ax, const float ay, const float az,
    const float gx, const float gy, const float gz,
    const float pitch, const float roll, const float yaw
) const {
    BotTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_BOT_TELEMETRY;
    msg.timestamp = timestamp;
    msg.dt = dt;
    msg.ax = ax;
    msg.ay = ay;
    msg.az = az;
    msg.gx = gx;
    msg.gy = gy;
    msg.gz = gz;
    msg.pitch = pitch;
    msg.roll = roll;
    msg.yaw = yaw;
    send(reinterpret_cast<uint8_t*>(&msg), sizeof(msg));
}

void OutboundCommandSender::sendPitchPidTelemetry(const PidController *pid) const {
    BotPidTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_PITCH_PID_TELEMETRY;
    msg.input = pid->lastInput;
    msg.setpoint = pid->lastSetpoint;
    msg.error = pid->lastError;
    msg.output = pid->lastOutput;
    msg.p_term = 0;
    msg.i_term = pid->integral;
    msg.d_term = pid->lastDerivative;
    send(reinterpret_cast<uint8_t*>(&msg), sizeof(msg));
}

void OutboundCommandSender::sendSpeedPidTelemetry(const PidController *pid) const {
    BotPidTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_SPEED_PID_TELEMETRY;
    msg.input = pid->lastInput;
    msg.setpoint = pid->lastSetpoint;
    msg.error = pid->lastError;
    msg.output = pid->lastOutput;
    msg.p_term = 0;
    msg.i_term = pid->integral;
    msg.d_term = pid->lastDerivative;
    send(reinterpret_cast<uint8_t*>(&msg), sizeof(msg));
}

void OutboundCommandSender::send(const uint8_t *data, const uint16_t len) const {
    if (!remoteIP) {
        Serial.println("No remote IP address");
        return;
    }

    udp->beginPacket(remoteIP, remotePort);
    udp->write(data, len);
    udp->endPacket();
}

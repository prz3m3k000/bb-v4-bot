#include "UdpCommandSender.h"
#include "Commands.h"

UdpCommandSender::UdpCommandSender(WiFiUDP *udp) {
    this->udp = udp;
}

void UdpCommandSender::setRemoteAddress(const IPAddress &ip, const uint16_t port) {
    this->remoteAddress = ip;
    this->remotePort = port;
}

void UdpCommandSender::sendBotTelemetry(
    const uint32_t timestamp,
    const uint32_t dt,
    const float ax,
    const float ay,
    const float az,
    const float pitch,
    const float roll,
    const float yaw
) const {
    BotTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_BOT_TELEMETRY;
    msg.timestamp = timestamp;
    msg.dt = dt;
    msg.ax = ax;
    msg.ay = ay;
    msg.az = az;
    msg.pitch = pitch;
    msg.roll = roll;
    msg.yaw = yaw;
    sendData(reinterpret_cast<uint8_t *>(&msg), sizeof(msg));
};

void UdpCommandSender::sendSpeedPidTelemetry(
    const uint32_t timestamp,
    const uint32_t dt,
    const float measurement,
    const float setpoint,
    const float p_term,
    const float i_term,
    const float d_term,
    const float output
) const {
    BotPidTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_SPEED_PID_TELEMETRY;
    msg.timestamp = timestamp;
    msg.dt = dt;
    msg.measurement = measurement;
    msg.setpoint = setpoint;
    msg.p_term = p_term;
    msg.i_term = i_term;
    msg.d_term = d_term;
    msg.output = output;
    sendData(reinterpret_cast<uint8_t *>(&msg), sizeof(msg));
}

void UdpCommandSender::sendPitchPidTelemetry(
    const uint32_t timestamp,
    const uint32_t dt,
    const float measurement,
    const float setpoint,
    const float p_term,
    const float i_term,
    const float d_term,
    const float output
) const {
    BotPidTelemetryMsg msg{};
    msg.cmd = DashboardCommandId::DB_PITCH_PID_TELEMETRY;
    msg.timestamp = timestamp;
    msg.dt = dt;
    msg.measurement = measurement;
    msg.setpoint = setpoint;
    msg.p_term = p_term;
    msg.i_term = i_term;
    msg.d_term = d_term;
    msg.output = output;
    sendData(reinterpret_cast<uint8_t *>(&msg), sizeof(msg));
}
void UdpCommandSender::sendData(const uint8_t *data, uint16_t len) const {
    if (!remoteAddress) {
        Serial.println("Data not send, no remote address set");
        return;
    }

    udp->beginPacket(this->remoteAddress, this->remotePort);
    udp->write(data, len);
    udp->endPacket();
}

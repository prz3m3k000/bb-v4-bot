#include <Arduino.h>

#include "app/InboundCommandReceiver.h"
#include "xyz/config/serial.h"
#include "xyz/config/wifi.h"
#include "xyz/config/mdns.h"
#include "xyz/config/m5.h"
#include "domain/Bot.h"
#include "domain/configuration/Configuration.h"
#include "domain/motion_processing/MotionProcessing.h"
#include "domain/stop_watch/StopWatch.h"

auto WIFI_SSID = "0xff0000";
auto WIFI_PASS = "4XTT8sH5";
auto HOSTNAME = "balancing-bot";
constexpr uint16_t PORT = 8080;
constexpr uint32_t COMMAND_RECEIVE_INTERVAL_MS = 100; // 100uS
constexpr uint32_t BOT_LOOP_INTERVAL_MS = 10; // 10uS

WiFiUDP *udp;

InboundCommandReceiver *commandReceiver;
OutboundCommandSender *commandSender;
PidController *speedPid;
PidController *pitchPid;
MotorBase *motorBase;
MotionProcessing *motionProcessing;
Bot *bot;

StopWatch *commandReceiverTimer;
StopWatch *botStepTimer;

void setup() {
    Configuration::initSerial(115200);
    Configuration::initWire(21, 22,400000);
    Configuration::initWiFi(WIFI_SSID, WIFI_PASS);
    Configuration::initMDns(HOSTNAME);
    // serial_init(115200);
    // wifi_init(WIFI_SSID, WIFI_PASS);
    // mdns_init(HOSTNAME);
    // m5_init();

    udp = new WiFiUDP();
    udp->begin(PORT);

    speedPid = new PidController();
    speedPid->setOutputRange(-7, 7);

    pitchPid = new PidController();
    pitchPid->setOutputRange(-3000, 3000);

    const auto *md1 = new MotorDefinition(18, 23, 25, 33, 32);
    const auto *md2 = new MotorDefinition(27, 26, 13, 12, 14);
    motorBase = new MotorBase(md1, md2);

    motionProcessing = new MotionProcessing(0.995);
    commandSender = new OutboundCommandSender(udp);

    bot = new Bot(speedPid, pitchPid, motorBase, motionProcessing, commandSender);
    commandReceiver = new InboundCommandReceiver(bot, udp);

    commandReceiverTimer = new StopWatch();
    botStepTimer = new StopWatch();
}

void loop() {
    if (commandReceiverTimer->elapsed() >= COMMAND_RECEIVE_INTERVAL_MS) {
        commandReceiver->receiveAndHandle();
        commandReceiverTimer->reset();
    }

    // if (botStepTimer->elapsed() >= BOT_LOOP_INTERVAL_US) {
    bot->nextStep();
    // botStepTimer->reset();
    // }
}

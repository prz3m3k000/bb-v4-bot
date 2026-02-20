#include "Configuration.h"

void Configuration::initWiFi(const char *ssid, const char *password) {
    Serial.println("WiFi:");

    WiFi.begin(ssid, password);

    Serial.print(" * connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print(" * connected, IP = ");
    Serial.println(WiFi.localIP());
}

void Configuration::initSerial(unsigned long baud) {
    Serial.begin(baud);
}

void Configuration::initWire(int sdaPin, int sclPin, uint32_t frequency) {
    Wire.begin(21, 22, 400000);
}

void Configuration::initMDns(const char* hostName) {
    if (!MDNS.begin(hostName)) {
        Serial.println("Failed to start mDNS");
        return;
    }
    Serial.print("mDNS = ");
    Serial.print(hostName);
    Serial.println(".local");
}
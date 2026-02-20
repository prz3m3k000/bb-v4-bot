#pragma once

#include <ESPmDNS.h>
#include <WiFi.h>
#include <Wire.h>

class Configuration {
public:
    static void initWiFi(const char *ssid, const char *password);

    static void initSerial(unsigned long baud);

    static void initWire(int sdaPin, int sclPin, uint32_t frequency);

    static void initMDns(const char* hostName);
};


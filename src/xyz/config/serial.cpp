#include "serial.h"
#include <Arduino.h>

void serial_init(int baud) {
    Serial.begin(baud);
    delay(250);
}
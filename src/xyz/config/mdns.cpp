#include "mdns.h"
#include <ESPmDNS.h>

void mdns_init(const char* hostname) {
    if (!MDNS.begin(hostname)) {
      Serial.println("Error starting mDNS");
      while (true) {
          delay(1000);
      }
  }

  Serial.print("mDNS responder started: ");
  Serial.print(hostname);
  Serial.println(".local");
}
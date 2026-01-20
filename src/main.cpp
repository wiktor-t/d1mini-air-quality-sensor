#include <Arduino.h>
#include <ESP8266WiFi.h>

void setup() {
  WiFi.begin("wifi_name", "wifi_password");
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void loop() {}

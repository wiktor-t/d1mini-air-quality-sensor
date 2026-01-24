#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

ESP8266WebServer server(80);
SoftwareSerial co2(D4, D3);

byte cmd[] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25};
int co2ppm = 0;

void readCO2() {
  while (co2.available()) co2.read();
  co2.write(cmd, 7);
  delay(150);
  if (co2.available() >= 7) {
    byte r[7];
    co2.readBytes(r, 7);
    if (r[0] == 0xFE && r[1] == 0x44) {
      if (r[3] == 0xFF) {
        co2ppm = r[4];
      } else {
        co2ppm = (r[3] << 8) | r[4];
      }
    }
  }
}

void handleRoot() {
  readCO2();
  server.send(
    200,
    "text/html",
    "<h1>Air Quality</h1><p>CO2: " + String(co2ppm) + " ppm</p>"
  );
}

void setup() {
  WiFi.begin("wifi_name", "wifi_password");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  co2.begin(9600);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
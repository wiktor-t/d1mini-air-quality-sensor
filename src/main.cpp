#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>

ESP8266WebServer server(80);

SoftwareSerial co2Serial(D4, D3);
SoftwareSerial pms(D5, D6);

byte co2Cmd[] = {0xFE, 0x44, 0x00, 0x08, 0x02, 0x9F, 0x25};

int co2ppm = 0;
int pm1 = 0;
int pm25 = 0;
int pm10 = 0;

void readCO2() {
  while (co2Serial.available()) {
    co2Serial.read();
  }

  co2Serial.write(co2Cmd, 7);
  delay(150);

  if (co2Serial.available() >= 7) {
    byte r[7];
    co2Serial.readBytes(r, 7);

    if (r[0] == 0xFE && r[1] == 0x44) {
      if (r[3] == 0xFF) {
        co2ppm = r[4];
      } else {
        co2ppm = (r[3] << 8) | r[4];
      }
    }
  }
}

void readPMS() {
  // simple pms read
  if (pms.available() > 32) {
    if (pms.read() == 0x42 && pms.read() == 0x4D) {
      byte data[30];
      pms.readBytes(data, 30);

      pm1 = (data[4] << 8) | data[5];
      pm25 = (data[6] << 8) | data[7];
      pm10 = (data[8] << 8) | data[9];
    }
  }
}

void handleRoot() {
  readCO2();
  readPMS();

  server.send(
    200,
    "text/html",
    "<h1>Air Quality</h1>"
    "<p>CO2: " + String(co2ppm) + " ppm</p>"
    "<p>PM1: " + String(pm1) + " ug/m3</p>"
    "<p>PM2.5: " + String(pm25) + " ug/m3</p>"
    "<p>PM10: " + String(pm10) + " ug/m3</p>"
  );
}

void setup() {
  WiFi.begin("wifi_name", "wifi_password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  co2Serial.begin(9600);
  pms.begin(9600);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}

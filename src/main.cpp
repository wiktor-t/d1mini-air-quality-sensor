#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot() {
  server.send(
    200,
    "text/html",
    "<p>ESP local web page test</p>"
  );
} 

void setup() {
  WiFi.begin("wifi_name", "wifi_password");
  while (WiFi.status() != WL_CONNECTED) delay(500);

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}



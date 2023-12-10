#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Adafruit_NeoPixel.h>
#include "SPIFFS.h"
#include "led.h"
#include "config.h"

#define LED_PIN 23
#define LED_COUNT 12


Config config;
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB);
AsyncWebServer server(80);


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup()
{
  Serial.begin(115200);
  const char *filename = "/config.json"; 
  readConfigFromSPIFFS(filename,config);
  ring.begin();
  ring.setBrightness(10);
 
  WiFi.begin(config.ssid.c_str(), config.password.c_str());
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting....");
  }
  Serial.println(WiFi.localIP());
  server.on("/ledon", HTTP_GET, [](AsyncWebServerRequest *request){ 
    turnLedOn(ring);
    request->send(200, "text/plain", "Led on"); 
  });
  server.on("/ledoff", HTTP_GET, [](AsyncWebServerRequest *request){ 
    turnLedOff(ring);
    request->send(200, "text/plain", "Led off"); 
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
}

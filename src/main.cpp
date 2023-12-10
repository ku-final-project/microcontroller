#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include "SPIFFS.h"

#define LED_PIN 23
#define LED_COUNT 12

struct Config {
  String ssid;
  String password;
};

Config config;
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB);
AsyncWebServer server(80);

void turnLedOn(Adafruit_NeoPixel ring)
{
  for (int i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, ring.Color(255, 255, 255));
  }
  ring.show();
}

void turnLedOff(Adafruit_NeoPixel ring)
{
  for (int i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, ring.Color(0, 0, 0, 0));
  }
  ring.show();
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void readConfigFromSPIFFS(const char *filename, Config &config){
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  File file = SPIFFS.open(filename);

  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  String json;
  while (file.available()) {
    json = file.readString();
  }
  const char* jsonString = json.c_str();
  DynamicJsonDocument doc(200);
  deserializeJson(doc, jsonString);

  const char* ssid = doc["ssid"];
  const char* password = doc["password"];
  config.ssid = String(ssid);
  config.password = String(password);
  file.close();
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

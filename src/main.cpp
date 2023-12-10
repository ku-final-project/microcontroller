#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "SPIFFS.h"

#define LED_PIN 23
#define LED_COUNT 12

String ssid,password;
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


void setup()
{
  Serial.begin(115200);
  ring.begin();
  ring.setBrightness(10);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  File file = SPIFFS.open("/config.txt");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  int i = 0;
  while (file.available()) {
    String s = file.readStringUntil('\n');
    if(i==0){
      ssid = s;
    }
    else {
      password = s;
    }
    i++;
  }
  file.close();

  Serial.printf("\'%s\'\n",ssid.c_str());
  Serial.printf("\'%s\'",password.c_str());
  WiFi.begin(ssid.c_str(), password.c_str());
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

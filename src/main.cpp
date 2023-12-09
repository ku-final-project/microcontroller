#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <ESPAsyncWebServer.h>

#define LED_PIN 23
#define LED_COUNT 12

const char *ssid = "A702";
const char *password = "420199355";
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
  WiFi.begin(ssid, password);
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

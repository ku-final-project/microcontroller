#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>
#include <Ultrasonic.h>
#include <AceRoutine.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "led.h"
#include "config.h"

#define LED_PIN 23
#define LED_COUNT 12
#define RELAY 22 
#define BUTTON 21
#define REED 13

int state = 0;

Config config;
Adafruit_NeoPixel ring(LED_COUNT, LED_PIN, NEO_GRB);
AsyncWebServer server(80);
Bounce debouncer = Bounce();
Ultrasonic ultrasonic(15, 2);	

COROUTINE(light) {
  COROUTINE_LOOP() {
    if(ultrasonic.read() < 60){
      turnLedOn(ring);
    }
    else {
    turnLedOff(ring);
    }
    COROUTINE_DELAY(1000);
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable detector
  Serial.begin(115200);
  pinMode(REED, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, 1);
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(25);
  const char *filename = "/config.json"; 
  readConfigFromSPIFFS(filename,config);
  ring.begin();
  ring.setBrightness(50);
  if (!WiFi.config(config.local_IP, config.gateway, config.subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(config.ssid.c_str(), config.password.c_str());
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting....");
  }
  Serial.println(WiFi.localIP());
  server.on("/led-on", HTTP_GET, [](AsyncWebServerRequest *request){ 
    turnLedOn(ring);
    request->send(200, "text/plain", "Led on"); 
  });
  server.on("/led-off", HTTP_GET, [](AsyncWebServerRequest *request){ 
    turnLedOff(ring);
    request->send(200, "text/plain", "Led off"); 
  });
  server.on("/open-door", HTTP_GET, [](AsyncWebServerRequest *request){ 
    digitalWrite(RELAY, 0);
    state = 1;
    request->send(200, "text/plain", "Open door"); 
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
  debouncer.update();
  if (debouncer.fell())
  {
    Serial.println("Begin Unlock...");
    digitalWrite(RELAY, 0);
    delay(1000);
    state = 1;
  }
  if (state == 1)
  {
    delay(500);
    if (!digitalRead(REED))
    {
      Serial.println("LOCKING");
      digitalWrite(RELAY, 1);
      state = 0;
      Serial.println("LOCKING finish");
      Serial.flush();
      delay(100);
    }
  }
  light.runCoroutine();
}

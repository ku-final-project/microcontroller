#include <Arduino.h>
#include <AceRoutine.h>
#include <Bounce2.h>
using namespace ace_routine;

int state = 0;

#define RELAY 15
#define BUTTON 10
#define REED 4
#define LED 2
// for esp32
// #define RELAY 23
// #define BUTTON 22
// #define REED 21
// #define LED 2

COROUTINE(serial)
{
  COROUTINE_LOOP()
  {
    Serial.println("Simulate Analog Read");
    COROUTINE_DELAY_SECONDS(2);
  }
}

Bounce debouncer = Bounce();

void setup()
{
  Serial.begin(115200);
  pinMode(REED, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(RELAY, 1);
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(25);
  digitalWrite(LED, 0);
}

void loop()
{
  serial.runCoroutine();
  debouncer.update();
  if (Serial.available())
  {
    String serial_send = Serial.readString();
    if (state == 0 && serial_send == "unlock")
    {
      Serial.println("Begin Unlock...");
      digitalWrite(RELAY, 0);
      delay(100);
      state = 1;
    }
    if (serial_send == "led_on")
    {
      digitalWrite(LED, 1);
    }
    else if (serial_send == "led_off")
    {
      digitalWrite(LED, 0);
    }
  }
  else if (debouncer.fell())
  {
    Serial.println("Begin Unlock...");
    digitalWrite(RELAY, 0);
    delay(100);
    state = 1;
  }
  else if (state == 1)
  {
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
}

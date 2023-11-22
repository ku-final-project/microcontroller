#include <Arduino.h>
#include <AceRoutine.h>
#include <Bounce2.h>
using namespace ace_routine;

int state = 0;

#define RELAY 15
#define BUTTON 10
#define REED 4
// #define LED 2
// for esp32
// #define RELAY 23
// #define BUTTON 22
// #define REED 21
// #define LED 2

// COROUTINE(led)
// {
//   COROUTINE_LOOP()
//   {
//     // Serial.println("led");
//     digitalWrite(LED, !digitalRead(RELAY));
//     COROUTINE_DELAY(1);
//   }
// }

Bounce debouncer = Bounce();

COROUTINE(lock)
{
  COROUTINE_LOOP()
  {
    if (state == 1)
    {
      if (!digitalRead(REED))
      {
        Serial.println("LOCKING");
        digitalWrite(RELAY, 1);
        state = 0;
        Serial.println("LOCKING finish");
        COROUTINE_DELAY(2000);
      }
    }
    COROUTINE_DELAY(1);
  }
}
COROUTINE(serial)
{
  COROUTINE_LOOP()
  {
    if (state == 0 && Serial.available()){
        String x = Serial.readString();
        Serial.println("Serial available is");
        // if(x == "unlock"){
          // Serial.println("begin unlock");
        digitalWrite(RELAY, 0);
        COROUTINE_DELAY(2000);
        state = 1;
        // }
    }
    else if (state == 1)
    {
      Serial.println("in state 1");
      if (!digitalRead(REED))
      {
        Serial.println("LOCKING");
        digitalWrite(RELAY, 1);
        state = 0;
        Serial.println("LOCKING finish");
        Serial.flush();
        COROUTINE_DELAY(2000);
      }
      COROUTINE_DELAY(1);
    }
    COROUTINE_DELAY(1);
  }
}

COROUTINE(press)
{
  static int buttonState = HIGH;
  static int prevButtonState = HIGH;

  COROUTINE_LOOP()
  {
    if (state == 0){
      // debouncer.update();
      // if(debouncer.fell()){
      //     digitalWrite(RELAY, 0);
      //     COROUTINE_DELAY(2000);
      //     state = 1;
      // }
      buttonState = digitalRead(BUTTON);
      if ((prevButtonState == HIGH) && (buttonState == LOW))
      {
        // primitive debouncing
        COROUTINE_DELAY(20);
        buttonState = digitalRead(BUTTON);
        if ((prevButtonState == HIGH) && (buttonState == LOW))
        {
          // Serial.println("begin unlock");
          digitalWrite(RELAY, 0);
          COROUTINE_DELAY(2000);
          state = 1;
          // Serial.println("state = 1");
        }
      }
      prevButtonState = buttonState;
      COROUTINE_DELAY(20);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON, INPUT);
  pinMode(REED, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  // pinMode(LED, OUTPUT);
  // pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(RELAY, 1);
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(25); 
  // digitalWrite(LED, 0);
  // CoroutineScheduler::setup();
}

void loop()
{
  // Serial.println(digitalRead(REED));
  // delay(100);
  // lock.runCoroutine();
  // serial.runCoroutine();
  // press.runCoroutine();
  // CoroutineScheduler::loop();
  debouncer.update();
  if (state == 0 && (Serial.available() || debouncer.fell())){
      String x = Serial.readString();
      // if(x == "unlock"){
        // Serial.println("begin unlock");
      digitalWrite(RELAY, 0);
      // COROUTINE_DELAY(2000);
      delay(100);
      state = 1;
      // }
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
      // COROUTINE_DELAY(2000);
      delay(100);
    }
  }
  
}

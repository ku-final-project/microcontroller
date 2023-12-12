#include "led.h"

void turnLedOn(Adafruit_NeoPixel &ring)
{
  for (int i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, ring.Color(255, 255, 255));
  }
  ring.show();
}

void turnLedOff(Adafruit_NeoPixel &ring)
{
  for (int i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, ring.Color(0, 0, 0, 0));
  }
  ring.show();
}
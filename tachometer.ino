#include <microLED.h>
#include "rpmcounter.h"
#include "trigger.h"

const int NUMLEDS = 60;
const int STRIP_PIN = 7;
const int MAX_RECENT=16;
const byte IGNITION_PIN = 4;

void ignition(uint32_t timestamp, void * arg);
void timeout(void *);
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;
RPMCounter<9, 2, 7500, 200000, 2> counter;
Trigger trigger(TRIGGER_FALLING, 200, ignition, NULL, 1000000, timeout, NULL);

void draw() {
  uint32_t rpm = counter.rpm();
  strip.leds[0] = mRGB(255, 0, 0);
  strip.leds[1] = mRGB(0, 255, 0);
  strip.leds[2] = mRGB(0, 0, 255);
  strip.leds[19] = mRGB(255, 255, 255);
  strip.show();
  Serial.println(rpm);
}

void timeout(void *) {
  Serial.println("Line timeout");
}

void ignition(uint32_t timestamp, void *) {
  counter.push(timestamp);
}

void setup() {
  Serial.begin(115200);
  pinMode(IGNITION_PIN, INPUT);

  strip.setBrightness(10);
  strip.clear();
  strip.show();
}

void loop() {
  trigger.update(digitalRead(IGNITION_PIN), micros());
}

#include <microLED.h>
#include "rpmcounter.h"
#include "trigger.h"

const int LOW_BORDER = 3500;
const int SWITCH_BORDER = 6800;
const int HIGH_BORDER = 7200;

const int NUMLEDS = 64;
const int STRIP_PIN = 7;
const int MAX_RECENT=16;
const byte IGNITION_PIN = 4;

void ignition(uint32_t timestamp, void * arg);
void timeout(void *);
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;
RPMCounter<9, 2, 8500, 200000, 2> counter;
Trigger trigger(TRIGGER_FALLING, 200, ignition, NULL, 1000000, timeout, NULL);

void draw(uint32_t rpm) {
  int bar = rpm / 125;
  mData color = mBlue;
  if (rpm > HIGH_BORDER) {
    color = mRed;    
  } else if (rpm > SWITCH_BORDER) {
    color = mYellow;
  } else if (rpm > LOW_BORDER) {
    color = mGreen;
  }

  for (int i = 0; i < NUMLEDS; i++) {
    if (i % 8 == 0) {
      strip.leds[i] = mWhite;
      continue;
    }

    if (i < bar) {
      strip.leds[i] = color;
    } else {
      strip.leds[i] = mBlack;
    }
  }
  
  strip.show();
}

void timeout(void *) {
//  Serial.println("Line timeout");
}

void ignition(uint32_t timestamp, void *) {
  counter.push(timestamp);
  draw(counter.rpm());
}

void setup() {
  Serial.begin(115200);
  pinMode(IGNITION_PIN, INPUT);

  strip.setBrightness(10);
  strip.clear();
  strip.show();
}

void loop() {
//  trigger.update(digitalRead(IGNITION_PIN), micros());
  for (int i = 0; i <= 8000; i+=10) {
    draw(i);
  }
  for (int i = 8000; i >= 0; i-=10) {
    draw(i);
  }
//  for (int i = 0; i < 64; i++) {
//    strip.leds[i] = mBlue;
//    strip.show();
//  }
}

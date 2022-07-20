#include <microLED.h>
#include "rpmcounter.h"
#include "trigger.h"

const int LOW_BORDER = 3500;
const int SWITCH_BORDER = 6500;
const int HIGH_BORDER = 7500;

const int NUMLEDS = 72;
const int STRIP_PIN = 7;
const int MAX_RECENT=16;
const byte IGNITION_PIN = 4;

const int L = 160;
const int H = 255;

mData mLightBlue   = mRGB(L, L, H);
mData mLightGreen  = mRGB(L, H, L);
mData mLightYellow = mRGB(H, H, L);
mData mLightRed    = mRGB(H, L, L);

void ignition(uint32_t timestamp, void * arg);
void timeout(void *);
microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;
RPMCounter<9, 2, 8500, 200000, 2> counter;
Trigger trigger(TRIGGER_FALLING, 200, ignition, NULL, 1000000, timeout, NULL);

void draw(uint32_t rpm) {
  int bar = rpm / 125;

  mData color = mBlue;
  mData divisionColor = mLightBlue;
  if (rpm > HIGH_BORDER) {
    color = mRed;    
    divisionColor = mLightRed;    
  } else if (rpm > SWITCH_BORDER) {
    color = mYellow;
    divisionColor = mLightYellow;
  } else if (rpm > LOW_BORDER) {
    color = mGreen;
    divisionColor = mLightGreen;
  }

  for (int i = 0; i < NUMLEDS; i++) {
    if (i < bar) {
      if ((i + 1) % 8 == 0) {
        strip.leds[i] = divisionColor;
      } else {
        strip.leds[i] = color;
      }
    } else {
      if ((i + 1) % 8 == 0) {
        strip.leds[i] = mWhite;
      } else {
        strip.leds[i] = mBlack;
      }
    }
  }
  
  strip.show();
}

void timeout(void *) {
  draw(0);
}

void ignition(uint32_t timestamp, void *) {
  counter.push(timestamp);
  draw(counter.rpm());
}

void setup() {
  Serial.begin(115200);
  pinMode(IGNITION_PIN, INPUT);

  strip.setBrightness(48);
  strip.clear();
  strip.show();

  for (int rpm = 0; rpm < 9000; rpm+= 50) {
    draw(rpm);
  }
  for (int rpm = 9000; rpm >= 0; rpm-= 50) {
    draw(rpm);
  }
}

void loop() {
  trigger.update(digitalRead(IGNITION_PIN), micros()); 
}

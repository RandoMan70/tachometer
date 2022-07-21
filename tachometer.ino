#include <microLED.h>
#include "rpmcounter.h"
#include "trigger.h"

const int LOW_BORDER = 3500;
const int SWITCH_BORDER = 6500;
const int HIGH_BORDER = 7500;

const int STRIPE_NUMLEDS = 72;
const int STRIPE_PIN = 7;
const int STIPE_BRIGHTNESS = 48;
const byte IGNITION_PIN = 4;

const int COUNTER_MAX_RPM = 8500;
const int COUNTER_IGNITION_SCALING = 2;
const int COUNTER_IGNITION_DEPTH = 8;
const long COUNTER_LATENCY_MKS = 200000;

const int TRIGGER_EDGE = TRIGGER_FALLING;
const long TRIGGER_ASSURE_MKS = 200;
const long TRIGGER_TIMEOUT_MKS = 1000000;

const int dL = 80;
const int dH = 128;

const int L = 160;
const int H = 255;

mData mDarkBlue   = mRGB(dL, dL, dH);
mData mDarkGreen  = mRGB(dL, dH, dL);
mData mDarkYellow = mRGB(dH, dH, dL);
mData mDarkRed    = mRGB(dH, dL, dL);

mData mLightBlue   = mRGB(L, L, H);
mData mLightGreen  = mRGB(L, H, L);
mData mLightYellow = mRGB(H, H, L);
mData mLightRed    = mRGB(H, L, L);

void ignition(uint32_t timestamp, void * arg);
void timeout(void *);
microLED< STRIPE_NUMLEDS, STRIPE_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> stripe;
RPMCounter<COUNTER_IGNITION_DEPTH + 1, COUNTER_IGNITION_SCALING, COUNTER_MAX_RPM, COUNTER_LATENCY_MKS, 2> counter;
Trigger trigger(TRIGGER_EDGE, TRIGGER_ASSURE_MKS, ignition, NULL, TRIGGER_ASSURE_MKS, timeout, NULL);

void draw(uint32_t rpm) {
  int bar = rpm / 125;

  mData color = mBlue;
  mData divisionColor = mLightBlue;
  mData inactiveColor = mDarkBlue;
  if (rpm > HIGH_BORDER) {
    color = mRed;    
    divisionColor = mLightRed;
    inactiveColor = mDarkRed;
  } else if (rpm > SWITCH_BORDER) {
    color = mYellow;
    divisionColor = mLightYellow;
    inactiveColor = mDarkYellow;
  } else if (rpm > LOW_BORDER) {
    color = mGreen;
    divisionColor = mLightGreen;
    inactiveColor = mDarkGreen;
  }

  for (int i = 0; i < STRIPE_NUMLEDS; i++) {
    if (i < bar) {
      if ((i + 1) % 8 == 0) {
        stripe.leds[i] = divisionColor;
      } else {
        stripe.leds[i] = color;
      }
    } else {
      if ((i + 1) % 8 == 0) {
        stripe.leds[i] = mWhite;
      } else {
        stripe.leds[i] = inactiveColor;
      }
    }
  }
  
  stripe.show();
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

  stripe.setBrightness(STIPE_BRIGHTNESS);
  stripe.clear();
  stripe.show();

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

#include <microLED.h>
#include "rpmcounter.h"

const int NUMLEDS = 60;
const int STRIP_PIN = 7;
const int MAX_RECENT=16;
const byte IGNITION_PIN = 4;

microLED< NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_LOW> strip;
RPMCounter<16, 7500, 2000, 4, 2> counter;
//
//const int32_t IGNITION_MIN_DELAY = 2000;
//const int32_t IGNITION_MAX_INTERVAL = 1000000;
//const int32_t IGNITION_MIN_COUNT = 4;
//
//uint32_t recent[MAX_RECENT] = {0};
//int recent_count = 0;
//
//bool ignition_noize(uint32_t now) {
//  return now - recent[0] < IGNITION_MIN_DELAY;
//}
//
//void ignition_push(uint32_t now) {
//  for (int idx = MAX_RECENT - 2; idx >= 0; idx--) {
//    recent[idx + 1] = recent[idx];
//  }
//
//  recent[0] = now;
//  recent_count ++;
//  
//  if (recent_count > MAX_RECENT) {
//    recent_count = MAX_RECENT;
//  }
//}
//
//void ignition_cleanup() {
//  for (int idx = recent_count - 1; idx >= 0; idx--) {
//    if (recent[0] - recent[idx] > IGNITION_MAX_INTERVAL) {
//      recent[idx] = 0;
//      recent_count --;
//    }
//  }  
//}
//
//void ignition_print() {
//  for (int idx = 0; idx < recent_count; idx++) {
//    Serial.print(recent[idx]);
//    Serial.write(" ");
//  }
//  Serial.write("\r\n");
//}
//
//uint32_t recent_interval() {
//  if (recent_count < IGNITION_MIN_COUNT) {
//    return 0;
//  }
//  
//  return recent[0] - recent[recent_count - 1];
//}
//
//uint32_t rpm() {
//  if (recent_count < IGNITION_MIN_COUNT) {
//     return 0;
//  }
//  return ((uint32_t)30000000 * (uint32_t) recent_count) / recent_interval() ;
//}

//uint32_t gauge() {
//  if (recent_count < IGNITION_MIN_COUNT) {
//     return 0;
//  }
//  return rpm() * 64/7500;
//}

//void ignition(uint32_t now) {
//  if (ignition_noize(now)) {
//    return;
//  }
//  Serial.println("--------------------");
//  ignition_push(now);
//  ignition_cleanup();
//  ignition_print();
//  Serial.println(recent_interval());
//  Serial.println(rpm());
//  Serial.println(gauge());
//
//}



int cnt = 0;
void ignition() {
  counter.push(micros());
  cnt ++;
}

void draw() {
  strip.leds[0] = mRGB(255, 0, 0);
  strip.leds[1] = mRGB(0, 255, 0);
  strip.leds[2] = mRGB(0, 0, 255);
  strip.leds[19] = mRGB(255, 255, 255);
  strip.show();
}

void setup() {
  Serial.begin(9600);
  pinMode(IGNITION_PIN, INPUT);

  strip.setBrightness(10);
  strip.clear();
  strip.show();
}

bool test_ignition() {
  static int old_value = digitalRead(IGNITION_PIN);
  int value = digitalRead(IGNITION_PIN);
  bool fired = old_value != value;
  old_value = value;
  return fired;
}


void loop() {
  Serial.write("--------\n");
  if (test_ignition()) {
    ignition();
  }
  Serial.print(cnt);
  Serial.write(" ");
  Serial.print(counter.rpm());
  Serial.write(" ");
  Serial.println(digitalRead(IGNITION_PIN));
  draw();
}

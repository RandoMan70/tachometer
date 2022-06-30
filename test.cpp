#ifdef RUN_TEST
#include "rpmcounter.h"
#include "trigger.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static RPMCounter<5, 2, 7500, 200000, 2> counter;

int absvalue(int v) {
  if (v < 0) {
    return -v;
  }

  return v;
}

void hit(uint32_t interval)  {
  static uint32_t micros = 0;

  micros += interval;
  counter.push(micros);

  uint32_t expected = 60000000 / interval / 2;
  uint32_t accounted = counter.rpm();
  if (expected > 400 && absvalue(int (expected - accounted)) > expected / 4) {
    printf("TEST FAIL!\n");
    printf("%u | %u | %u | %i | %i\n", micros, accounted, expected, absvalue(expected - accounted), expected / 5);
  }
}

void test_rpm() {

    uint32_t interval = 100000;
    for (int cycle = 0; cycle < 3; cycle++) {
      for (;interval >= 4000; interval -= 505 - rand() % 10) hit(interval);
      for (int i = 0; i < 20; i++) {
        hit(interval - 5 + rand() % 10);
      }
      for (;interval < 200000; interval += 505 - rand() % 10) hit(interval);
      for (int i = 0; i < 20; i++) {
        hit(interval - 5 + rand() % 10);
      }
    }
}


void timeout(void *) {
  printf("timeout\n");
}

void ignition(uint32_t timestamp, void *) {
  printf("fire %u\n", timestamp);
}

void test_trigger() {
  Trigger trigger(TRIGGER_FALLING, 200, ignition, NULL, 1000000, timeout, NULL);

  struct {
    uint32_t ts;
    int value;
  } line_data[] = {
   {1000000, 1},
   {1000010, 1},
   {1000100, 1},
   {1000110, 0},
   {1000112, 1},
   {1000114, 0},
   {1000122, 1},
   {1000132, 0},
   {1000143, 0},
   {1000154, 0},
   {1001354, 0},
   {1002354, 0},
   {1004354, 0},
   {2005354, 0},
   {3006354, 0},
   {4007354, 0},
   {5000000, 0},
   {5000001, 1},
   {5000020, 1},
   {5000220, 1},
   {5001001, 0},
   {5001020, 0},
   {5001220, 0},
   {0, 0}
  };

  for (int idx = 0; line_data[idx].ts != 0; idx ++) {
//    printf("%i, %u\n", line_data[idx].value, line_data[idx].ts);
    trigger.update(line_data[idx].value, line_data[idx].ts);
  }

}

int main() {
  test_rpm();
  test_trigger();  
}
#endif

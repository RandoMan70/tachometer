#include <stdint.h>
#include "fifo.h"

#ifndef RPMCOUNTER_MAX_DEPTH
#define RPMCOUNTER_MAX_DEPTH 16
#endif

class rpmCounter {
public:
  rpmCounter(uint32_t max_rpm, uint32_t latency_mks, uint32_t smooth, uint32_t hits_per_rotate);
  void push(uint32_t now_mks);
  int rpm();
  
private:
  GyverFIFO<uint32_t, RPMCOUNTER_MAX_DEPTH> hits;
  uint32_t min_delay;
  uint32_t max_interval;
  uint32_t min_count;
  uint32_t division;
};

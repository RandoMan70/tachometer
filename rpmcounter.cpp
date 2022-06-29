#include "rpmcounter.h"

rpmCounter::rpmCounter(uint32_t max_rpm, uint32_t latency_mks, uint32_t smooth, uint32_t hits_per_rotate) {
  min_delay = (uint32_t)(60000000) / (max_rpm * 2);
  max_interval = latency_mks;
  min_count = smooth > RPMCOUNTER_MAX_DEPTH ? RPMCOUNTER_MAX_DEPTH : smooth;
  division = hits_per_rotate;
}
void rpmCounter::push(uint32_t now_mks) {
  // Filter noize
  if (hits.available() && (now_mks - hits.peek() < min_delay)) {
    return;
  }

  // Cleanup obsoleted records
  while (hits.available() && (now_mks - hits.peek() > max_interval)) {
    hits.read();
  }

 // Prepare space
  if (!hits.availableForWrite()) {
    hits.read();
  }

  // Intsert record
  hits.write(now_mks);
}

int rpmCounter::rpm() {
  // We should have at least min_count hits
  uint32_t count = hits.available();
  if (count < min_count) {
    return 0;
  }

  uint32_t interval = hits.peek_newest() - hits.peek();
  return ((uint32_t)60000000 / division * count) / interval ;
}

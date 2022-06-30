#include <stdint.h>
#include "fifo.h"

template <uint32_t SIZE, uint32_t SCALING, uint32_t MAX_RPM, uint32_t MAX_LATENCY, uint32_t MIN_DEPTH>
class RPMCounter {
public:
  RPMCounter() {
    min_delay = (uint32_t)(60000000) / (MAX_RPM * SCALING * 2);
    max_interval = MAX_LATENCY;
    min_count = MIN_DEPTH > SIZE ? SIZE : MIN_DEPTH;
    scaling = SCALING;
  }

  void push(uint32_t now_mks) {
    // Filter noize
    if (history.available() && (now_mks - history.newest() < min_delay)) {
      return;
    }
  
    // Cleanup obsoleted records
    while (history.available() && (now_mks - history.oldest() > max_interval)) {
      history.read();
    }
  
   // Free space on overflow
    if (!history.availableForWrite()) {
      history.read();
    }
  
    // Insert record
    history.write(now_mks);
  }
  uint32_t rpm() {
    // We should have at least min_count entries
    uint32_t count = history.available();
    if (count < min_count) {
      return 0;
    }
  
    uint32_t interval = history.newest() - history.oldest();
    uint32_t rpm = (uint32_t)60000000 / scaling * (count - 1) / interval;
    return rpm;
  }
  
private:
  ExtendedGyverFIFO<uint32_t, SIZE> history;
  uint32_t min_delay;
  uint32_t max_interval;
  uint32_t min_count;
  uint32_t scaling;
};

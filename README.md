# Addressed led stripe tachometer based on arduino

### Description
Initially written for WS2812, but can be used with any other stripe, supported by underlying libraries.
Due to physical restrictions of stripe protocol it's limit is nearly 18k hits per second using 72 leds (144-led densed stripes are recommended). Less leds - higher hits processing rate.
For generic 4-stroke engine having ignition signal twice per engine rotate it's nearly 9kRPM. Can be optimized for higher RPMs by having signal from one spark plug only.

### Dependencies and 3rd-party code
https://github.com/GyverLibs/microLED
https://github.com/GyverLibs/GyverFIFO

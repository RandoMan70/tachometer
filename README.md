# Addressed led stripe tachometer based on arduino

### Description
Initially written for WS2812, but can be used with any other stripe, supported by underlying libraries.
Due to physical restrictions of stripe protocol it's limit is nearly 18k hits per second using 72 leds (144-led densed stripes are recommended). Less leds - higher hits processing rate.
For generic 4-stroke engine having ignition signal twice per engine rotate it's nearly 9kRPM. Can be optimized for higher RPMs by having signal from one spark plug only.

### Operation

### Featuers
- Input signal noise protection
- Accounting smoothing
- Shifting range
- Colored idle scale

### Colors meaning
- Blue - low RPMs, recommended to shift down.
- Green - range of maximum effeciensy.
- Yellow - high RPMs, recommended to shift up.
- Red - too high RPMs, immediately shift up.

### Configuration

Parameter | Default value | Meaning
-----------------------------------
LOW_BORDER | 3500 | Lowest RPMs for effecient acceleration range
SWITCH_BORDER | 6500 | Highest RPMs for effecient acceleration range
HIGH_BORDER | 7500 | Red zone RPMs
STRIPE_NUMLEDS | 72 | Amount of leds in stripe
STRIPE_PIN | 7 | Output pin for led
IGNITION_PIN | 4 | Input pin for ignition signal
COUNTER_MAX_RPM | 8500 | Maximum accounted RPMs
COUNTER_IGNITION_SCALING | 2 | How many signal edges happen for single engine rotation
COUNTER_IGNITION_DEPTH | 8 | How many hits timestamps to keep for smoother accounting
COUNTER_LATENCY_MKS | 200000 | Maximum lag in RPMs accounting. Protects on low RPMs
TRIGGER_EDGE | TRIGGER_FALLING | Signal edge (rising or falling)
TRIGGER_ASSURE_MKS | 200 | How long to wait before assure that edge is triggered
TRIGGER_TIMEOUT_MKS | 1000000 | Reset RPMs to zero when nothing happens

### Dependencies and 3rd-party code
- https://github.com/GyverLibs/microLED
- https://github.com/GyverLibs/GyverFIFO

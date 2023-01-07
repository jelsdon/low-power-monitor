# low-power-monitor
Low power consumption ESP32 based environment to mqtt sensor

## Goals
To gather environment (temperature/humidity) data and send over WiFi offgrid solution

## Parts
* DFROBOT Firebeetle ESP32 ($15.08)
* Polymer Lithium Ion Battery LiPo 1100mAh 3.7V 603450 ($13.90)
* DFRobot Solar Lipo Charger, 3.7 V Battery, 4.4 V to 6 V Supply ($8.10 AUD)
* BME280 5V 3.3V Digital Sensor ($5.45 AUD)
* 6V 1W Mini Solar Panel ($2.15 AUD)

### Notes
* Firebeetle was chosen for it's low power consumption, particularily under deep sleep
* Parts should be ordered without status LED's (or otherwise removed) 

# Power usage analytics
The project averages 1mAs per reading, recording once every minute.
With a 1100mAh battery, this allows (In theory) a maximum of 3960000 recordings without charging.

With a 6V 1W solar panel, 9 seconds per day of sun is required to keep the battery full.
> 1000 X 1 (Wh) X 6 (v) = 166.mAh ; Solar output
> 0.4 (mAh/day usage) / 166.666 = 8.64 seconds

![Power Consumpion](usage.png?raw=true "Power Consumption")
__note__ Power consumption shown in image above was taken whilst BME280 sensor had attached LED. It has since been removed
and draws 9-12uAh under deep sleep.

## Schematics
TODO

# Planned changes
Monitor battery level

# Learnings
* 1100mAh battery is too large for this project. It may last years without sun before running flat

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
The project averages 92mAs per reading.
With a 1100mAh battery, this allows theoretical a maximum of 718 readings without charge (92mAs * 60 readings per hour = 5520mAs, or 1.53mAh average consumption).
In practice this will be lower due to efficiencies and minimal voltage level required to power the circuit.

With a 6V 1W solar panel, 13 minutes per day of direct sun is required to keep the battery full.
> (1Wh * 1000) / 6 (v) = 166.mAh ; Solar output
> 1.53mAh * 24h = 36 (mAh per day consumed)
> 36mAh / 166.666mAh = 13 minutes

![Power Consumpion during measurement/transmission](usage.png?raw=true "Power Consumption during measurement/transmission")

_Power consumption during measurements / transmission. 1 mA is represented as 1mV_

![Power Consumpion during deep-sleep](deep-sleep.png?raw=true "Power Consumption during deep sleep")

_Power consumption during deep sleep. 1 uA is represented as 1mV_

Measurements taken using a [CurrentRanger](https://lowpowerlab.com/shop/product/152) and [JYETech Wave2](https://jyetech.com/wave2-2-channel-portable-oscilloscope/)

## Schematics
![High Level Connectivity](high-level-connectivity.png?raw=true "High level connectivity")

|         From              |           To              |
|:---                       |           :---            |
| Charge Controller PWR IN+ | Solar Panel+              |
| Charge Controller PWR IN- | Solar Panel-              |
| Charge Controller BAT+    | LiPo+                     |
| Charge Controller BAT-    | LiPo-                     |
| Charge Controller VOUT+   | Firebeetle Li-ION+        |
| Charge Controller VOUT-   | Firebeetle Li-ION-        |
| BME280 VCC                | Firebeetle 3v3            |
| BME280 GND                | Firebeetle GND            |
| BME280 SCL                | Firebeetle SCL/IO22       |
| BME280 SDA                | Firebeetle SDA/IO21       |

# Planned changes
Monitor battery level

# Learnings
* ~~1100mAh battery is too large for this project. It may last years without sun before running flat~~ Continue to check over my maths

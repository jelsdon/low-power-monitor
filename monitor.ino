#define uS_TO_S_FACTOR 1000000   /* Conversion factor for micro seconds to seconds     */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in seconds)           */
#define WDT_TIMEOUT 10           /* Restart after this many seconds (watchdog restart) */
#include <WiFi.h>
#include "WiFiClientSecure.h"
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <esp_task_wdt.h>


#define SEALEVELPRESSURE_HPA (1013.25f)

RTC_DATA_ATTR int recordCounter = 0;

/************************* WiFi Access Point *********************************/
// TODO Move to header
#define WLAN_SSID ""
#define WLAN_PASS ""

/************************* Adafruit.io Setup *********************************/
// TODO Move to header

// DNS lookup had negligible impact over IP on consumption
// Flexibility to move the server is currently more preferable
#define AIO_SERVER      "homeassistant"

// Using port 8883 for MQTTS
#define AIO_SERVERPORT  1883


#define AIO_USERNAME ""
#define AIO_KEY      ""

WiFiClient client;
Adafruit_BME280 bme;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish MQTT_TEMP = Adafruit_MQTT_Publish(&mqtt, "firebeetle/bme280/temperature");
Adafruit_MQTT_Publish MQTT_HUMI = Adafruit_MQTT_Publish(&mqtt, "firebeetle/bme280/humidity");
Adafruit_MQTT_Publish MQTT_PRES = Adafruit_MQTT_Publish(&mqtt, "firebeetle/bme280/pressure");
Adafruit_MQTT_Publish MQTT_ALTI = Adafruit_MQTT_Publish(&mqtt, "firebeetle/bme280/altitude");


// DNS lookup had negligible impact over IP on consumption
// TODO: Move to DHCP as flexibility is currently more preferable over consumption
IPAddress local_IP(10, 0, 0, 1);     // Device IP
IPAddress gateway(10, 0, 0, 254);      // Network Gateway
IPAddress subnet(255, 255, 255, 0);    // Device Subnet
IPAddress primaryDNS(10, 0, 0, 254);   // Network DNS Server


/*************************** Sketch Code ************************************/

void setup() {
  float temp;
  float humi;
  float pres;
  float alti;
  unsigned status;


  esp_task_wdt_init(WDT_TIMEOUT, true);
  esp_task_wdt_add(NULL);
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);

  status = bme.begin();
  if (!status) {
    while (1) delay(10);
  }

  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );

  bme.takeForcedMeasurement();
  temp = bme.readTemperature();
  humi = bme.readHumidity();
  pres = bme.readPressure();
  //alti = bme.readAltitude(SEALEVELPRESSURE_HPA);

  WiFi.config(local_IP, gateway, subnet, primaryDNS);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(25);
  }

  MQTT_connect();
  esp_task_wdt_reset();

  MQTT_TEMP.publish(temp);
  MQTT_HUMI.publish(humi);
//  MQTT_ALTI.publish(alti);    /* Don't waste time on altitude as statically deployed */
  MQTT_PRES.publish(pres);

  // wait a couple seconds to avoid rate limit
  delay(80);
  esp_deep_sleep_start();
}


void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       mqtt.disconnect();;
       retries--;
       // Let the watchdog take care of a reboot
       if (retries == 0) {
         while (1);
       }
  }

}

void loop(){}

#include <Arduino.h>

#include "HC_WiFi.h"
#include "HC_ntp.h"
#include "HC_DHTSensors.h"
// #include "HC_Server.h"
#include "HC_http.h"

#include "HC_utils.h"
#include "secrets.h"

int sleepInterval = 2;

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);

  HC_WiFi::connect((const char*) SECRETS::ssid, (const char*) SECRETS::password, SECRETS::hostname);
  HC_ntp::init("CET-1CEST,M3.5.0,M10.5.0/3");
  HC_DHTSensors.setup(D1, DHTesp::DHT22);
  HC_http.init(SECRETS::server, SECRETS::port);

  // HC_Server::init();
  // HC_Server::discoveryService();
}

void callback() {
  Serial.println();
  HC_ntp::printLocalTime();
  HC_DHTSensors.updateHcValues();
  HC_DHTSensors.displayTempHumidity();
  HC_http.postTmpHumData();
  Serial.println("-------- Task done --------");
}

void loop() {
  HC_utils::deepSleepLoop(callback, sleepInterval);
}



// #define ledPin 2 /**/

// void setup()
// {
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, HIGH);

// }

// void loop()
// {
//   digitalWrite(ledPin, LOW);
//   delay(5000);
//   ESP.deepSleep(10e6); /* Sleep for 5 seconds */
// }
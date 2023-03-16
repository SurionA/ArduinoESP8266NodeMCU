#include <Arduino.h>

#include "HC_WiFi.h"
#include "HC_ntp.h"
#include "HC_DHTSensors.h"
#include "HC_Server.h"
#include "secrets.h"

// int sleepInterval = 2;
int jobDone = 0;

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(true);


  pinMode(D7, OUTPUT);
  digitalWrite(D7, HIGH);
  pinMode(D8, OUTPUT);
  digitalWrite(D8, HIGH);

  HC_WiFi::connect((const char*) SECRETS::ssid, (const char*) SECRETS::password, SECRETS::hostname);

  HC_ntp::init("CET-1CEST,M3.5.0,M10.5.0/3");

  HC_DHTSensors.setup(D0, DHTesp::DHT11);

  HC_Server::init();
  HC_Server::discoveryService();
  HC_ntp::printLocalTime();
}
using Callback = void (*)();

void loopSleep(Callback callback, int sleepInterval ) {
  int minutes = HC_ntp::getMinutes();

  if (fmod(minutes, sleepInterval) == 0 && jobDone ==0) {
    jobDone=1;
    callback();
  } else {
    jobDone=0;

    int sleepDelay = HC_ntp::getDelaySyncTime(sleepInterval);
    Serial.println();
    Serial.println("Estimated sleep: " +  (String) sleepDelay);

    delay(sleepDelay);
  }
}


void callback() {
  HC_DHTSensors.updateHcValues();
  Serial.println();
  HC_ntp::printLocalTime();
  Serial.println("JOB IS DONE !!");
}

void loop() {
  loopSleep(callback, 2);
}
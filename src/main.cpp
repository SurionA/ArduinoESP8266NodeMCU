#include <Arduino.h>

#include "HC_WiFi.h"
#include "HC_ntp.h"
#include "HC_DHTSensors.h"
#include "HC_Server.h"
#include "secrets.h"

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

void loop() {
  HC_DHTSensors.updateHcValues();
  int minutes = HC_ntp::getSencond();

        if (fmod(minutes, 15)==0) {

        Serial.print("minutes: ");
        Serial.println(minutes);
        } else {
        Serial.println("Sleep");
        }

  // int delay = 0;

    // switch (true)
    //   {
    //   case minutes < 15:
    //     delay = 15 - minutes
    //      break;
    //   case 5:
    //   case 6:
    //   case 7:
    //   case 8:
    //      cout << "Now you need a house. ";
    //      break;
    //   default:
    //      cout << "What are you? A peace-loving hippie freak? ";
    //   }
  // HC_DHTSensors.displayTempHumidity();
  delay(1000);
}
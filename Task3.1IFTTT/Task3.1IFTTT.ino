#include <WiFiNINA.h>
#include "secrets.h"
#include <BH1750.h>
#include <Wire.h>

//please enter your sensitive data in the Secret tab
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

WiFiClient client;
BH1750 lightMeter;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME1   = "https://maker.ifttt.com/trigger/Sunlight_detect/with/key/B0qCTISleDYJuKqyAVaJL" ;
String PATH_NAME2 = "https://maker.ifttt.com/trigger/Sunlight_undetected/with/key/B0qCTISleDYJuKqyAVaJL";
String queryString = "?value1=57&value2=25";

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);

  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  lightMeter.begin();
  Serial.println(F("Test begin"));

  // connect to web server on port 80:
  if (client.connect(HOST_NAME, 80)) {
    // if connected:
    Serial.println("Connected to server");
    delay(500);
  }
  else {// if not connected:
    Serial.println("connection failed");
  }
}

void sendFTTTRequest(String path){
  if(client.connect(HOST_NAME, 80)){
        client.println("GET " + path + queryString + " HTTP/1.1");
        client.println("Host: " + String(HOST_NAME));
        client.println("Connection: close");
        client.println(); // end HTTP header


        while (client.connected()) {
          if (client.available()) {
          // read an incoming byte from the server and print it to serial monitor:
            char c = client.read();
            Serial.print(c);
            }
        }
        client.stop();
        Serial.println("\nDisconnected from server");
  }  else {
    Serial.println("Connection to server failed");
  }
}

void loop() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light Level: ");
  Serial.print(lux);
  Serial.print(" lx");


  if(lux > 250) {
    Serial.println("Sunlight Detected - Sending Notification");
    sendFTTTRequest(PATH_NAME1);
  }else{
    Serial.println("Sunlight Undetected - Sending Notification");
    sendFTTTRequest(PATH_NAME2);
  }

  delay(20000);
}

#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include "secrets.h"
#include <HCSR04.h>

// Wi-Fi credentials (defined in secrets.h)
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// MQTT broker info
const char broker[] = "broker.emqx.io";
const int port = 1883;
const char waveTopic[] = "SIT210/wave10";
const char patTopic[] = "SIT210/pat";

// Distance sensor pins
const int triggerPin = 13;
const int echoPin = 12;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

// WiFi and MQTT clients
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

// Timing variables
const long interval = 8000;
unsigned long previousMillis = 0;

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(3000);
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void connectToMQTT() {
  Serial.print("Connecting to MQTT broker: ");
  Serial.println(broker);

  while (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());
    delay(5000);
  }

  Serial.println("MQTT connected!");
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Wait for USB serial connection

  connectToWiFi();
  connectToMQTT();
}

void loop() {
  mqttClient.poll(); // Keep MQTT alive

  // Reconnect if WiFi or MQTT gets disconnected
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  if (!mqttClient.connected()) {
    connectToMQTT();
  }

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    Serial.println("Measuring distance...");

    float distance = distanceSensor.measureDistanceCm();
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 20.0) {
      Serial.println("Wave detected! Sending MQTT message...");
      mqttClient.beginMessage(waveTopic);
      mqttClient.print("DulajWave");
      mqttClient.endMessage();
      Serial.println("Message sent.");
    } else if (distance < 40.0) {
      Serial.println("Pat detected! Sending MQTT message...");
      mqttClient.beginMessage(patTopic);
      mqttClient.print("DulajPat");
      mqttClient.endMessage();
      Serial.println("Message sent.");
    }
  }
}

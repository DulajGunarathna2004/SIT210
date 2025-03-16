#include "DHT.h"
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"

#define DHTPIN 2        
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;   
char pass[] = SECRET_PASS;   
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);
  while (!Serial) { }
  
  Serial.println(F("Initializing DHT sensor..."));
  dht.begin();
  
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("WiFi connected.");
  
  ThingSpeak.begin(client);
}

void loop() {
  // Ensure WiFi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    Serial.println("WiFi reconnected.");
  }
  
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);

  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);
  float heatIndexF = dht.computeHeatIndex(temperatureF, humidity);

  Serial.print("Humidity: "); Serial.print(humidity);
  Serial.print("% Temperature: "); Serial.print(temperatureC);
  Serial.print("°C ( "); Serial.print(temperatureF);
  Serial.print("°F ) Heat Index: "); Serial.print(heatIndexC);
  Serial.print("°C ( "); Serial.print(heatIndexF);
  Serial.println("°F )");
  
  // Send data to ThingSpeak
  ThingSpeak.setField(1, humidity);
  ThingSpeak.setField(2, temperatureC);
  ThingSpeak.setField(3, temperatureF);
  ThingSpeak.setField(4, heatIndexC);
  
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (response == 200) {
    Serial.println("ThingSpeak update successful.");
  } else {
    Serial.println("Error updating ThingSpeak: " + String(response));
  }
  
  delay(20000); // Wait 20 seconds before next update
}

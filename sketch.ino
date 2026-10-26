#include "DHTesp.h"
#include <WiFi.h>
#include "ThingSpeak.h"
const int DHT_PIN = 15;
const char* ssid = "Wokwi-GUEST";
const char* password = "";
unsigned long channelNumber = 3551384;
const char* apiKey = "";
const char* universityID = "12327280";

DHTesp dhtSensor;
WiFiClient clientObject;
void setup() {
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password, 6);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");
  ThingSpeak.begin(clientObject);
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  Serial.print("University ID: ");
  Serial.println(universityID);
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  if (WiFi.status() == WL_CONNECTED) {
    ThingSpeak.setField(1, data.temperature);
    ThingSpeak.setField(2, data.humidity);

    int status = ThingSpeak.writeFields(channelNumber, apiKey);

    if (status == 200) {
      Serial.println("Data sent successfully!");
    } else {
      Serial.print("ThingSpeak error: ");
      Serial.println(status);

      if (status == -301) {
        Serial.println("Could not connect to ThingSpeak.");
      } else if (status == -401) {
        Serial.println("Update rate limit reached.");
      } else if (status == 404) {
        Serial.println("Check the channel number and API key.");
      }
    }
  } else {
    Serial.println("WiFi disconnected!");
  }
  Serial.println("--------------------");
  delay(20000);
}

#include "DHT.h"                 // Import DHT library

#define DHT_PIN D5               // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11            // Define sensor type

DHT dht(DHT_PIN, DHTTYPE);       // Initialize DHT sensor

void setup() {
  Serial.begin(9600);            // Start serial monitor
  dht.begin();                   // Start DHT sensor
}

void loop() {
  delay(2000);                   // Wait a few seconds between measurements

  float h = dht.readHumidity();      // Read humidity
  float t = dht.readTemperature();   // Read temperature in Celsius

  // Check if any reads failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" °C  ");

  // Print humidity
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}
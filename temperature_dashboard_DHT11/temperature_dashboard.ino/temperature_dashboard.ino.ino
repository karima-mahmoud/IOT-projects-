
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#include "webpage.h"

const char* ssid = "";
const char* password ="";

#define DHTPIN D4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

const int MAX_POINTS = 20;
float tempHistory[MAX_POINTS];
float humHistory[MAX_POINTS];
String labelHistory[MAX_POINTS];
int historyCount = 0;

unsigned long lastReadTime = 0;
const unsigned long readInterval = 3000;

float currentTemp = 0.0;
float currentHum = 0.0;

String getStatusText(float temp) {
  if (temp < 20) return "Cool";
  if (temp < 30) return "Normal";
  return "Hot";
}

void addReading(float t, float h) {
  String label = String(millis() / 1000) + "s";

  if (historyCount < MAX_POINTS) {
    tempHistory[historyCount] = t;
    humHistory[historyCount] = h;
    labelHistory[historyCount] = label;
    historyCount++;
  } else {
    for (int i = 0; i < MAX_POINTS - 1; i++) {
      tempHistory[i] = tempHistory[i + 1];
      humHistory[i] = humHistory[i + 1];
      labelHistory[i] = labelHistory[i + 1];
    }
    tempHistory[MAX_POINTS - 1] = t;
    humHistory[MAX_POINTS - 1] = h;
    labelHistory[MAX_POINTS - 1] = label;
  }
}

void updateSensorReading() {
  if (millis() - lastReadTime >= readInterval) {
    lastReadTime = millis();

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (!isnan(h) && !isnan(t)) {
      currentTemp = t;
      currentHum = h;
      addReading(t, h);
    }
  }
}

void handleRoot() {
  server.send_P(200, "text/html", MAIN_page);
}

void handleData() {
  updateSensorReading();

  String json = "{";
  json += "\"temperature\":" + String(currentTemp, 1) + ",";
  json += "\"humidity\":" + String(currentHum, 1) + ",";
  json += "\"environment\":\"" + getStatusText(currentTemp) + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

void handleHistory() {
  updateSensorReading();

  String json = "{";
  json += "\"labels\":[";
  for (int i = 0; i < historyCount; i++) {
    json += "\"" + labelHistory[i] + "\"";
    if (i < historyCount - 1) json += ",";
  }
  json += "],";

  json += "\"temps\":[";
  for (int i = 0; i < historyCount; i++) {
    json += String(tempHistory[i], 1);
    if (i < historyCount - 1) json += ",";
  }
  json += "],";

  json += "\"hums\":[";
  for (int i = 0; i < historyCount; i++) {
    json += String(humHistory[i], 1);
    if (i < historyCount - 1) json += ",";
  }
  json += "]";
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    currentTemp = t;
    currentHum = h;
    addReading(t, h);
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/history", handleHistory);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  updateSensorReading();
  server.handleClient();
}
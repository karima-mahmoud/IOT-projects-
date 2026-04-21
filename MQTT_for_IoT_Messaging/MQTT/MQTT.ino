#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "AhmedNoman";
const char* password = "AaB123@&321";
const char* mqtt_server = "192.168.1.9"; 

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) message += (char)payload[i];

  if (message == "ON") digitalWrite(12, HIGH); 
  else if (message == "OFF") digitalWrite(12, LOW);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP8266_Karima")) {
      client.subscribe("home/led");
    } else {
      delay(5000);
    }
  }
}

void setup() {
  pinMode(12, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
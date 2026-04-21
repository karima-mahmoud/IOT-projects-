#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


#define WIFI_SSID "AhmedNoman"
#define WIFI_PASSWORD "AaB123@&321"


#define DATABASE_SECRET "1Yvj6cfWzmXmDDcWtrjUJfqmmhTTAfmLZSgcyI1i"
#define DATABASE_URL "https://control-led-e6719-default-rtdb.firebaseio.com/"

#define LED_PIN 12

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi connected");

  
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;


  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  if (Firebase.ready()) {
    if (Firebase.getString(fbdo, "/led")) {
      String ledState = fbdo.stringData();
      Serial.println("LED state: " + ledState);

      if (ledState == "ON") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("Command sent: HIGH");
      } else if (ledState == "OFF") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("Command sent: LOW");
      }
    } else {
      Serial.print("Read failed: ");
      Serial.println(fbdo.errorReason());
    }
  }
  delay(1000);
}
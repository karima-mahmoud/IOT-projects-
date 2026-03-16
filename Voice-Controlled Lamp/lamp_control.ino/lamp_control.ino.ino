#define LED_PIN D6

void setup() {

  Serial.begin(9600);      // Start serial communication
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  if (Serial.available()) {

    char cmd = Serial.read();

    if (cmd == '1') {
      digitalWrite(LED_PIN, HIGH);   // Turn ON LED
    }

    else if (cmd == '0') {
      digitalWrite(LED_PIN, LOW);    // Turn OFF LED
    }

  }

}
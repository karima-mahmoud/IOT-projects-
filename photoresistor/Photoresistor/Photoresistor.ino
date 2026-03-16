int photoresistor = 0;      // holds the photoresistor reading
int threshold = 750;        // threshold value
#define LED_PIN D1          // LED on D1 (GPIO5)

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  photoresistor = analogRead(A0);
  Serial.println(photoresistor);

  if (photoresistor < threshold) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(100);
}
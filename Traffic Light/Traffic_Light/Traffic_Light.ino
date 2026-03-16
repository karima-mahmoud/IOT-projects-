#include <LiquidCrystal.h>

LiquidCrystal lcd(D1, D2, D5, D6, D7, D0);

#define RED_LED    D3
#define YELLOW_LED D4
#define GREEN_LED  D8

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("Traffic Light");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // RED
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RED LIGHT");
  lcd.setCursor(0, 1);
  lcd.print("STOP");
  delay(5000);

  // RED + YELLOW
  digitalWrite(RED_LED, HIGH);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RED+YELLOW");
  lcd.setCursor(0, 1);
  lcd.print("READY");
  delay(2000);

  // GREEN
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GREEN LIGHT");
  lcd.setCursor(0, 1);
  lcd.print("GO");
  delay(5000);

  // YELLOW
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("YELLOW LIGHT");
  lcd.setCursor(0, 1);
  lcd.print("WAIT");
  delay(2000);
}
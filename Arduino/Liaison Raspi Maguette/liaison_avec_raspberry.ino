#include <Wire.h>
#include <LiquidCrystal.h>

// Setting the LCD shields pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

String nom = "sur l'Arduino ";
String movement;

void setup() {
  Serial.begin(9600); 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Salut Raspi");
}
void loop() {
  readSerialPort();
  Serial.println("Position :");
  Serial.println("----------");
  Serial.println(F("GCP1 X - get current position X"));
  Serial.println(F("GCP1 Y - get current position Y"));
  Serial.println(F("GCP2 X Y - get current position X and Y"));
  Serial.println("");
  delay(1000);

  if (movement != "") {
    sendData();
  }
  delay(1000);
}

void readSerialPort() {
  movement = "";
  if (Serial.available()) {
    delay(10);
    while (Serial.available() > 0) {
      movement += (char)Serial.read();
    }
    Serial.flush();
  }
}

void sendData() {
  //write data
  Serial.print(nom);
  Serial.print(" commande recue : ");
  Serial.print(movement);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("r:");
  lcd.print(movement);
}

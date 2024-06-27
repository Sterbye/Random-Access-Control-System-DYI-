#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define GREEN_LED 6
#define RED_LED 7
#define BUZZER 8

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to your I2C address
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // Initialize serial communications
  Serial.begin(9600);
  SPI.begin();            // Init SPI bus
  mfrc522.PCD_Init();     // Init MFRC522
  lcd.begin();
  lcd.backlight();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  // Randomly grant or deny access
  int accessGranted = random(2); // 0 or 1

  lcd.clear();
  if (accessGranted == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Access Granted");
    digitalWrite(GREEN_LED, HIGH);
    tone(BUZZER, 1000); // Sound the buzzer at 1000 Hz
    delay(2000);
    digitalWrite(GREEN_LED, LOW);
    noTone(BUZZER); // Stop the buzzer
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER, 500); // Sound the buzzer at 500 Hz
    delay(2000);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER); // Stop the buzzer
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan your card");
  delay(1000);
}

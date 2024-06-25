#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define GREEN_LED 6
#define RED_LED 7

LiquidCrystal_I2C lcd(0x27, 16, 2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{

    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    lcd.begin(16, 2);

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);

    lcd.setCursor(0, 0);
    lcd.print("Priloz kartu");
}

void loop()
{

    // Nova Karta
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    // Ukaz UID na serial monitore
    Serial.print("UID tag :");
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println();
    Serial.print("Message : ");
    content.toUpperCase();

    // Nahodne udel pristup alebo zamietni True or false alebo 0 or 1
    int accessGranted = random(2);

    lcd.clear();
    if (accessGranted == 1)
    {
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Pristup povoleny");
        digitalWrite(GREEN_LED, HIGH);
        delay(2000);
        digitalWrite(GREEN_LED, LOW);
    }
    else
    {
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Kontrola");
        digitalWrite(RED_LED, HIGH);
        delay(2000);
        digitalWrite(RED_LED, LOW);
    }

    // Reset
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Priloz kartu");
    delay(1000);

    lcd.noBacklight();
}
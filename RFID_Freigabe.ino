/*
 * Pin Layout MFRC522 -> Arduino Uno
 * ----------------------------------
 *             RFID          Arduino
 * Typ         MFRC522       Uno/101
 * Signal      PIN           PIN
 * ----------------------------------
 * RST/Reset   RST           9
 * SPI SS      SDA(SS)       10
 * SPI MOSI    MOSI          11
 * SPI MISO    MISO          12
 * SPI SCK     SCK           13
 * ----------------------------------
 */


#include <SPI.h>                  // SPI Bibliothek einbinden
#include <MFRC522.h>              // MFRC522 Bibliothek einbinden         

#define RST_PIN   9               // Definiert SPI Interface RST_PIN (Reset) auf Arduino Pin 9
#define SS_PIN    10              // Definiert SPI Interface SS_PIN (SLave Select) auf Arduino Pin 10
MFRC522 mfrc522(SS_PIN, RST_PIN); // Erzeugt "mfrc522" Objekt.

byte karteUID[] = {0xE6, 0xB8, 0xBD, 0x1A};

int karteOK = 3;

int karte_check = false;


void setup() 
{
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();

    pinMode(3, OUTPUT);

}

void loop() 
{
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() ) {    // Nur wenn Karte gefunden UND gelesen werden konnte wird IF-Funktion ausgeführt
      Serial.print("Gelesene UID:");    // Gibt Text "Gelsene UID:" über USB aus.
      for (byte i = 0; i< mfrc522.uid.size; i++)
      {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "); // Wenn das Byte kleiner 16, dann schreibt er eine führende 0, ansonsten nur ein Leerzeichen
        Serial.print(mfrc522.uid.uidByte[i], HEX);                // Ausgabe in Hexadezimal Werten...
      }
      Serial.println();     // Zeilenumbruch im Seriellen Monitor (an der Ausgabe)

      //Beginn der Vergleichsoperation
      karte_check = true;
      for (int j=0; j<4; j++) 
      {
        if (mfrc522.uid.uidByte[j] != karteUID[j]) 
        {
          karte_check = false;
        }
      }

    if (karte_check == 1) 
    {
      digitalWrite(karteOK, HIGH);
    }
    if (karte_check != 1) 
    {
      int i = 0;
      while(i<5)
      {
        digitalWrite(karteOK, HIGH);
        delay(100);
        digitalWrite(karteOK, LOW);
        delay(100);
        i++;
      }
    }

      

      mfrc522.PICC_HaltA(); // Versetzt die Karte in Temporären Ruhemodus um eventuell weitere Karte zu lesen
      delay(2000);          // 1 Sekunde Wartezeit…
    digitalWrite (karteOK, LOW);
    
  }

}

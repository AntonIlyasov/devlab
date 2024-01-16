#include "FS.h"
#include <SD.h>
#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "GyverButton.h"
#include "iarduino_RTC.h"
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Arduino.h>

int writeCount = 148;

//PN532////
#define PN532_IRQ   (50)
#define PN532_RESET (51)  // Not connected by default on the NFC Shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

//btn////
#define BTN_PWD1 2
GButton btnPWD1(BTN_PWD1);

//data///
char newChar    = 'i';
String result   = "";
String tagData  = "";

//WiFi////
String boxID                    = "asdrt";
String secret_key               = "a086d0ee0aff004b5034fcdb04ec400c";

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); 

  btnPWD1.setType(LOW_PULL);

  // nfc setup
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN53x board");
    while (!versiondata){
    versiondata = nfc.getFirmwareVersion();
      delay(500);
    }
  }
  Serial.println("PN53x board");
  Serial.println("SUCCESS BOX SETUP");
}

void loop() {
  btnPWD1.tick();
  checkSingleClick();
  checkDoubleClick();
}

void checkSingleClick(){
  if (btnPWD1.isSingle()) {
    Serial.println("btnPWD1.isSingle()");
    setNFCMarkToCard();
  }
}

void checkDoubleClick(){
  if (btnPWD1.isDouble()) {
    Serial.println("btnPWD1.isDouble()");
    readNFC();
  }
}

bool setNFCMarkToCard(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1000);
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1000);

  if (success) {
    Serial.print("  UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    if (uidLength == 4){
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);
      if (success){
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data_to_write[20] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
        
        String writeCountStr = String(writeCount);

        for (int i = 0; i < writeCountStr.length(); i++){
          data_to_write[19 - i] = writeCountStr[writeCountStr.length() - 1 - i];
          Serial.print("i = ");
          Serial.println(i);
          Serial.print("data_to_write[19 - i] = ");
          Serial.println(data_to_write[19 - i]);
          Serial.print("writeCountStr[writeCountStr.length() - 1 - i] = ");
          Serial.println(writeCountStr[writeCountStr.length() - 1 - i]);
        }

        uint8_t data2writeForBlock4[16] = {'0'};
        uint8_t data2writeForBlock5[4]  = {'0'};

        for(int i = 0; i < 16; i++){
          data2writeForBlock4[i] = data_to_write[i];
        }

        for(int i = 0; i < 4; i++){
          data2writeForBlock5[i] = data_to_write[i + 16];
        }

        nfc.mifareclassic_WriteDataBlock(4, data2writeForBlock4);
        nfc.mifareclassic_WriteDataBlock(5, data2writeForBlock5);
        writeCount++;
        // uint8_t data_1[] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
        // nfc.mifareclassic_WriteDataBlock(4, data_1);

        // uint8_t data_2[] = {'0', '0', '0', '0'};
        // nfc.mifareclassic_WriteDataBlock(5, data_2);
        Serial.println("SUCCESS WRITING");
        return true;
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
        return false;
      }
    }
    else{
      Serial.println("This doesn't seem to be an Mifare Classic tag (UUID length != 4 bytes)!");
      return false;
    }
  } else {
    Serial.println("cant read tag");
    return false;
  }
}

bool readNFC(){
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 1000);
  
  if (success) {
    if (uidLength == 4){
      uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      // uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
      // Start with block 4 (the first block of sector 1) since sector 0
      // contains the manufacturer data and it's probably better just
      // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

      if (success){
        Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
        String read_data = "";
        // Try to read the contents of block 4 & 5
        for(int i = 4; i <= 5; i++){
          success = nfc.mifareclassic_ReadDataBlock(i, data);
          Serial.print("data: ");
          for(int kk = 0; kk < 16; kk++){
            Serial.print(data[kk]);
            Serial.print(" ");
          }
          Serial.println();

          if (success){
            printHexCharAsOneLine(data, 16);
            Serial.print("tagData: ");
            Serial.println(tagData);

            if (i == 4){
              Serial.print("data1: ");
              Serial.println(tagData.substring(tagData.length() - 5, tagData.length()));
              read_data += tagData.substring(tagData.length() - 5, tagData.length());
            }
            if (i == 5){
              Serial.print("data2: ");
              Serial.println(tagData.substring(0, 4));
              read_data += tagData.substring(0, 4);
            }
            tagData = "";
            // nfc.PrintHexChar(data, 16);
          }
          else
          {
            Serial.println("Ooops ... unable to read the requested block.  Try another key?");

            return false;
          }
        }
        //do smth
        Serial.println("read_data = ");
        Serial.println(read_data);

        result = "";
        result = "{\"box_id\":\"";
        result += boxID;
        result += "\", \"mark_id\":\"";
        result += read_data;
        result += "\", \"event_time\":\"";
        result += String(123123);
        // result += "\", \"secret_key\":\"";
        // result += secret_key;
        result += "\"}";
        // {"box_id":"asdfv", "mark_id":"444444444", "event_time":"123123"}
        Serial.println(result);
        return true;
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
        return false;
      }
    } else{
      Serial.println("This doesn't seem to be an Mifare Classic tag (UUID length != 4 bytes)!");
      return false;
    }
  } else {
    Serial.println("cant read tag");
    return false;
  }
}

void printHexCharAsOneLine(const byte *data, const uint32_t numBytes) {
  for (uint32_t szPos = 0; szPos < numBytes; szPos++) {
    if (data[szPos] > 0x1F){
      newChar = ((char)data[szPos]); //makes the character into a variable
      tagData += newChar; //adds that character to the result string
    }
  }
}
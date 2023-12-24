#define DEBUG_ON 1
#define DEBUG_OFF 0
#define MODE DEBUG_OFF

// #if MODE == DEBUG_ON
// #elif MODE == DEBUG_OFF
// #endif

#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_PN532.h>
#include "GyverButton.h"
#include "iarduino_RTC.h"
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "UnixTime.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* rootCACertificate = \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n";                                                 // левый какой-то сертификат
    
String unixTime             = "";
unsigned long unixTimeLong  = 0;

struct Data_Time{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
};

unsigned long activeTime = 0;
const String boxID  = "asdfv";
const String secret_key = "a086d0ee0aff004b5034fcdb04ec400c";

//acum//
#define BAT_CHARGE 34

//SPI//
#define SCK   (18)
#define MOSI  (23)
#define MISO  (19)
#define SD_SS (5)
SPIClass SPI_1(VSPI);

//PN532////
#define PN532_IRQ   (50)
#define PN532_RESET (51)  // Not connected by default on the NFC Shield
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

//btn////
#define BTN_PWD1 2
GButton btnPWD1(BTN_PWD1);

//RTC////
iarduino_RTC RTC(RTC_DS1302, 27, 25, 26);

//data///
char newChar    = 'i';
String result   = "";
String tagData  = "";
bool start      = false;

//RGB////
#define B 4
#define G 33
#define R 32

const uint8_t red[3]    = {255,0,0};    //Индикация ошибок
const uint8_t green[3]  = {0,255,0};    //100-50% charge
const uint8_t yellow[3] = {255,255,0};  //50-25% charge
const uint8_t orange[3] = {255,128,0};  //25-10% charge
const uint8_t off[3]    = {0,0,0};
const uint8_t rgb_on[3] = {255,255,255};
const uint8_t blue[3]   = {0,0,255};
const uint8_t purple[3] = {255,0,255};

//GSM
// #define RXD2 16
// #define TXD2 17

void setup(void){
  
  // Serial.begin(115200);
  // while (!Serial) delay(10); 
  
  btnPWD1.setType(LOW_PULL);

  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  RGB_write(rgb_on);

 // SD card setup
 if(!SD.begin(SD_SS)){
  // Serial.println("Card Mount Failed");
  while (!SD.begin(SD_SS)) {
    RGB_error();
    delay(500);
  }
  RGB_write(rgb_on);
 }
//  Serial.println("SD Card Mounted");

 if (!SD.exists("/id.txt")) {
  File myFile = SD.open("/id.txt", FILE_WRITE);
  myFile.close();
 }
    
 // nfc setup
 nfc.begin();
 uint32_t versiondata = nfc.getFirmwareVersion();
 if (!versiondata) {
  //  Serial.println("Didn't find PN53x board");
   while (!versiondata){
    versiondata = nfc.getFirmwareVersion();
     RGB_error();
     delay(500);
   }
   RGB_write(rgb_on);
 }
//  Serial.println("PN53x board");

  // sim card setup
  sim_card_setup();
  RGB_write(rgb_on);

  //rtc setup
  RTC.begin();
  int a = random(1, 5);
  if (a == 1){
    setTime();
  }
  
  RGB_write(off);
  // Serial.println("SUCCESS BOX SETUP");
  activeTime = millis();
}

void checkTimeForSleeping(){
  unsigned long currentTime = millis();
  if (currentTime - activeTime > 60000){
    activeTime = millis();
    // Serial.println("SLEEP");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 1);
    esp_light_sleep_start();
  }
}

void checkSingleClick(){
  if (btnPWD1.isSingle()) {
    // Serial.print("RTC.gettimeUnix = ");
    // Serial.println(RTC.gettimeUnix());
    activeTime = millis();
    // Serial.println("btnPWD1.isSingle()");
    if(readNFC()) sendDataToGSM();
  }
}

void checkDoubleClick(){
  if (btnPWD1.isDouble()) {
    activeTime = millis();
    // Serial.println("btnPWD1.isDouble()");
    show_charge(get_voltage(1), 730, 675, 642);
  }
}

void loop(void) {
  btnPWD1.tick();
  checkTimeForSleeping();
  checkSingleClick();
  checkDoubleClick();
}

void sim_card_setup(){
  Serial.begin(115200, SERIAL_8N1);
  int simCardFail = 0;
  String RespCodeStr = "";
  do{
    if (simCardFail > 2){
      RGB_error();
    }
    // Serial.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
    Serial.println("AT+CGACT=1,1"); // Activate net
    updateSerial();
    Serial.println("AT+CIICR");
    updateSerial();
    if (simCardFail > 2){
      RGB_error();
    }
    Serial.println("AT+CREG?");
    delay(1500);
    RespCodeStr = "";
    while (Serial.available()>0) {
      RespCodeStr += char(Serial.read());
    }
    // Serial.print("RespCodeStr = ");
    // Serial.println(RespCodeStr);
    simCardFail++;
  } while (!(RespCodeStr.indexOf("+CREG") >= 0));
}

void parseToTime(String input){
  // Serial.print("input = ");
  // Serial.println(input);
  String application_json = "application/json";
  int indexOfAppl = input.indexOf(application_json);
  int indexUnixTime = indexOfAppl + application_json.length();
  unixTime = input.substring(indexUnixTime, indexUnixTime + 12);
  // Serial.print("unixTime = ");
  // Serial.println(unixTime);
  // Serial.print("unixTime.length() = ");
  // Serial.println(unixTime.length());
  unixTimeLong = strtoul(unixTime.c_str(), NULL, 10);
  // Serial.print("unixTimeLong = ");
  // Serial.println(unixTimeLong); 
  RTC.settimeUnix(unixTimeLong);
}

void setTime(){

  // Serial.println("Configuring time...");
  Serial.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  updateSerial();
  Serial.println("AT+CIICR");
  updateSerial();
  Serial.println("AT+CGACT=1,1");
  updateSerial();
  Serial.println("AT+TERMHTTP");// Send data request to the server
  updateSerial();
  Serial.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial.println("AT+INITHTTP"); //The basic adhere network command of Internet connection
  updateSerial();
  Serial.println("AT+HTTPGET=\"http://185.241.68.155:8001/timeUnix\"");// Connect to the server then the server will send back former data
  delay(10000);

  String RespCodeStr = "";
  while (Serial.available()>0) {
    RespCodeStr += char(Serial.read());
  }
  // Serial.print("RespCodeStr = ");
  // Serial.println(RespCodeStr);
  // Serial.println("EndRespCodeStr");

  parseToTime(RespCodeStr);

  Serial2.println("AT+TERMHTTP");// Send data request to the server
  delay(1500);
}

void sendDataToSD(String fileName, String data, bool ledOn){
  File myFile = SD.open(fileName, FILE_APPEND);
  if (myFile) {
    myFile.println(data);
    // Serial.println(data + " send to " + fileName);
    #if MODE == DEBUG_OFF
    if (ledOn){
      RGB_success();
    }
    #endif
  }
  else {
    // Serial.println("Can`t open file " + fileName);
    #if MODE == DEBUG_OFF
    if (ledOn){
      RGB_error();
    }
    #endif
  }
  myFile.close();
}

void renameFile(){
  File myFile = SD.open("/id.txt", FILE_WRITE);
  File myFile2 = SD.open("/id2.txt");
  while (myFile2.available()){
    String buffer = myFile2.readStringUntil('\n');
    buffer.trim();
    myFile.println(buffer);
  }
  SD.remove("/id2.txt");
  myFile.close();
  myFile2.close();
}


void updateSerial()
{
  delay(1500);
  // while(Serial.available()) {
  //   Serial.write(Serial.read());//Data received by Serial will be outputted by Serial}
  // }
}

bool sendToGSM(String data, bool ledOn){
  Serial.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  updateSerial();
  Serial.println("AT+CIICR");
  updateSerial();
  Serial.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial.println("AT+HTTPINIT"); //The basic adhere network command of Internet connection
  updateSerial();
  Serial.println("AT+HTTPSSETCRT=0");
  updateSerial();
  Serial.println(rootCACertificate);
  delay(1500);
  Serial.println("AT+HTTPDATA");// Connect to the server then the server will send back former data
  updateSerial();
  Serial.println("AT+HTTPPARA=\"CID\",\"1\"");//Set PDP parameter
  updateSerial();
  Serial.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");//Activate PDP; Internet connection is available after successful PDP activation
  updateSerial();
  Serial.println("AT+HTTPPARA=\"URL\",\"http://185.241.68.155:8001/send_data\"");//Get local IP address
  updateSerial();
  Serial.println("AT+HTTPDATA");// Connect to the server then the server will send back former data
  updateSerial();
  Serial.println(data);// Send data request to the server
  delay(2000);
  Serial.write(26);// Terminator
  delay(2000);
  Serial.println("AT+HTTPACTION=1");// Send data request to the server
  delay(2000);
  Serial.println("AT+HTTPTERM");// Send data request to the server
  delay(1500);
  String RespCodeStr = "";
  while (Serial.available()>0) {
    RespCodeStr += char(Serial.read());
  }
  if (!RespCodeStr.isEmpty() && RespCodeStr.indexOf("200") >= 0){
    if (ledOn){
      RGB_success();
    }
    // Serial.println("SUCCESS gsm sending");
    return true;
  } else {
    #if MODE == DEBUG_ON
    if (ledOn){
      RGB_error();
    }
    #endif
    // Serial.println("error gsm sending");
    return false;
  }
}

void sendDataToGSM(){
  RGB_write(yellow);
  activeTime = millis();
  int failSendCount = 0;

  if(!sendToGSM(result, 1)){
    RGB_write(yellow);
    sendDataToSD("/id.txt", result, 1);
    RGB_write(yellow);
    failSendCount++;
    // Serial.print("failSendCount = ");
    // Serial.println(failSendCount);
    RGB_write(off);
    return;
  }
  RGB_write(yellow);
  activeTime = millis();
  // Send data from SD to wifi
  File myFile = SD.open("/id.txt", FILE_READ);
  if (myFile){
    // Serial.println("/id.txt:");
    while (myFile.available()){
      activeTime = millis();
      String buffer = myFile.readStringUntil('\n');      // Считываем с карты весь дотекст в строку до 
                                                          // символа окончания + перевод каретки (без удаления строки)
      if(!sendToGSM(buffer, 0)){
        RGB_write(yellow);
        buffer.trim();
        sendDataToSD("/id2.txt", buffer, 0);
        RGB_write(yellow);
        failSendCount++;
      }
      RGB_write(yellow);
    }
  } else {
    // Serial.println("error opening /id.txt");
  }
  SD.remove("/id.txt");
  myFile.close();
  renameFile();
  // Serial.print("failSendCount = ");
  // Serial.println(failSendCount);
  RGB_write(off);
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
      uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

      // Start with block 4 (the first block of sector 1) since sector 0
      // contains the manufacturer data and it's probably better just
      // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

      if (success){
        // Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
        uint8_t data[16];
        String read_data = "";
        // Try to read the contents of block 4 & 5
        for(int i = 4; i <= 5; i++){
          success = nfc.mifareclassic_ReadDataBlock(i, data);
          if (success){
            printHexCharAsOneLine(data, 16);
            if (i == 4) read_data += tagData.substring(tagData.length() - 5, tagData.length());
            if (i == 5) read_data += tagData.substring(0, 4);
            tagData = "";
            nfc.PrintHexChar(data, 16);
          }
          else
          {
            // Serial.println("Ooops ... unable to read the requested block.  Try another key?");
            RGB_error();
            return false;
          }
        }
        //do smth
        // Serial.println("read_data = ");
        // Serial.println(read_data);

        result = "";
        result = "{\"box_id\":\"";
        result += boxID;
        result += "\", \"mark_id\":\"";
        result += read_data;
        result += "\", \"event_time\":\"";
        result += String(RTC.gettimeUnix());
        // result += "\", \"secret_key\":\"";
        // result += secret_key;
        result += "\"}";
        // {"box_id":"asdfv", "mark_id":"444444444", "event_time":"123123"}
        // Serial.println(result);
        RGB_success();
        return true;
      }
      else
      {
        // Serial.println("Ooops ... authentication failed: Try another key?");
        RGB_error();
        return false;
      }
    } else{
      // Serial.println("This doesn't seem to be an Mifare Classic tag (UUID length != 4 bytes)!");
      RGB_error();
      return false;
    }
  } else {
    // Serial.println("cant read tag");
    RGB_error();
    return false;
  }
}

void printHexCharAsOneLine(const byte *data, const uint32_t numBytes) {
  uint32_t szPos;
  for (szPos = 0; szPos < numBytes; szPos++) {
    if (data[szPos] > 0x1F){
      newChar = ((char)data[szPos]); //makes the character into a variable
      tagData += newChar; //adds that character to the result string
    }
  }
}

void RGB_write(byte r_val, byte g_val, byte b_val){
  analogWrite(R, r_val);
  analogWrite(G, g_val);
  analogWrite(B, b_val);
}

void RGB_write(const uint8_t* color) {
  analogWrite(R, color[0]);
  analogWrite(G, color[1]);
  analogWrite(B, color[2]);
}

void RGB_error(){
  RGB_write(red);
  delay(300);
  RGB_write(off);
  delay(300);
  RGB_write(red);
  delay(300);
  RGB_write(off);
}

void RGB_success(){
  RGB_write(green);
  delay(300);
  RGB_write(off);
  delay(300);
  RGB_write(green);
  delay(300);
  RGB_write(off);
}

int get_voltage(bool debug_mode) {
  int voltage = analogRead(BAT_CHARGE);
  if(debug_mode) {
    //  Serial.print("VOLTAGE: analogPin: ");
    //  Serial.print(voltage);
    //  Serial.print(" Real: ");
     voltage = (25306+100*voltage)/323;
    //  Serial.println(voltage);
  }
  return voltage;
}

/*Показывает заряд батарейки, зажигая светодиод в соответствии с границами, соответствующими 50,25,10 процентам */
void show_charge(int voltage, const int bnd_50, const int bnd_25, const int bnd_10) {
  if (voltage < bnd_10) {
    RGB_write(red);
  }
  else if (voltage < bnd_25) {
    RGB_write(orange);
  }
  else if (voltage < bnd_50) {
    RGB_write(yellow);
  }
  else RGB_write(green);
  delay(1000);
  RGB_write(off);
}

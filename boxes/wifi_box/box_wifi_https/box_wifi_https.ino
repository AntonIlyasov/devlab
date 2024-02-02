#define DEBUG_ON 1
#define DEBUG_OFF 0
#define MODE DEBUG_OFF

// #if MODE == DEBUG_ON
// #elif MODE == DEBUG_OFF
// #endif

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
#include <Arduino_JSON.h>

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* rootCACertificate = \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "-----END CERTIFICATE-----\n";                                                

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//RGB////
#define B 4
#define G 33
#define R 32

unsigned long activeTime = 0;

//WiFi////
String boxID                    = "";
String secret_key               = "a086d0ee0aff004b5034fcdb04ec400c";
String serverName               = "https://box-dev.dvlb.ru/server/send_data";
String boxActivateServerName    = "https://box-dev.dvlb.ru/server/boxes/box/activate";
const char *esp32_wifi_ssid     = "cleaning box";
const char *esp32_wifi_password = "cleaningbox";

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

const uint8_t red[3]    = {255,0,0};    //Индикация ошибок
const uint8_t green[3]  = {0,255,0};    //100-50% charge
const uint8_t yellow[3] = {255,255,0};  //50-25% charge
const uint8_t orange[3] = {255,128,0};  //25-10% charge
const uint8_t off[3]    = {0,0,0};
const uint8_t rgb_on[3] = {255,255,255};
const uint8_t blue[3]   = {0,0,255};
const uint8_t purple[3] = {255,0,255};

AsyncWebServer server(80);
const char* LOGIN_1     = "login1";
const char* PASSWORD_1  = "password1";
const char* LOGIN_2     = "login2";
const char* PASSWORD_2  = "password2";
const char* LOGIN_3     = "login3";
const char* PASSWORD_3  = "password3";

// HTML web page to handle 3 input fields (input1, input2, input3, input4, input5, input6)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Wifi 1<br>
    login: <input type="text" name="login1"> password: <input type="text" name="password1"><br><br>
    Wifi 2<br>
    login: <input type="text" name="login2"> password: <input type="text" name="password2"><br><br>
    Wifi 3<br>
    login: <input type="text" name="login3"> password: <input type="text" name="password3"><br><br>
    <input type="submit" value="OK">
  </form>
</body></html>)rawliteral";

const char index_html_success[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <meta charset="UTF-8">
  <title>Success ESP Init</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
  Успешное завершение настройки. Перезагрузите устройство. 
</body></html>)rawliteral";

String inputMessage = "";
bool userInput = false;

void server_setup(){
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    inputMessage = "";

    inputMessage = request->getParam(LOGIN_1)->value();
    inputMessage += ";";
    inputMessage += request->getParam(PASSWORD_1)->value();
    inputMessage += "\n";

    inputMessage += request->getParam(LOGIN_2)->value();
    inputMessage += ";";
    inputMessage += request->getParam(PASSWORD_2)->value();
    inputMessage += "\n";

    inputMessage += request->getParam(LOGIN_3)->value();
    inputMessage += ";";
    inputMessage += request->getParam(PASSWORD_3)->value();
    inputMessage += "\n";

    Serial.println("inputMessage = ");
    Serial.println(inputMessage);

    String inputParam;
    request->send_P(200, "text/html", index_html_success);
    userInput = true;
  });
  server.onNotFound(notFound);
  server.begin();
  Serial.println("Server started");
}

bool setBoxIdFile(){

  // Wifi settings
  WiFiMulti wifiMulti;
  network_config(wifiMulti); //пишем все ssid и пароли из конфига
  wifi_connecting(wifiMulti);//и подключаемся

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client){
    Serial.println("client SUCCESS");
    client->setCACert(rootCACertificate);
    HTTPClient https;
    https.begin(*client, boxActivateServerName);
    
    String result = "";
    result = "{\"secret_key\":\"";
    result += secret_key;
    result += "\"}";
    Serial.println(result);
    // https.addHeader("Content-Type", "application/json");
    https.addHeader("secret-key", secret_key);
    int httpResponseCode = https.GET();
    Serial.println(httpResponseCode);
    String payload = "{}";

    Serial.print("httpResponseCode = ");
    Serial.println(httpResponseCode);

    if (httpResponseCode == 200){
      Serial.print("HTTPS Response code: ");
      Serial.println(httpResponseCode);
      payload = https.getString();
      Serial.print("payload: ");
      Serial.println(payload);
      JSONVar myObject = JSON.parse(payload);
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return false;
      }
      Serial.print("JSON object = ");
      Serial.println(myObject);
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar key = myObject.keys();
      JSONVar value = myObject[key[0]];
      Serial.print(key[0]);
      Serial.print(" = ");
      Serial.println(value);
      boxID = String(JSON.stringify(value));
      boxID.remove(0, 1);
      boxID.remove(boxID.length() - 1, 1);
      Serial.print("box_id: ");
      Serial.println(boxID);
      https.end();
      if (boxID == "") return false;
      Serial.println("\nOpen box_id_file file to write...");
      File box_id_file = SD.open("/box_id_file.txt", FILE_WRITE);
      if (!box_id_file) {
        Serial.println("\nCAN'T OPEN box_id_file FILE !");
        return false;
      }
      box_id_file.println(boxID);
      box_id_file.close();
      return true;
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
      https.end();
      return false;
    }
  } else{
    Serial.println("client ERROR");
    return false;
  }
}

bool setConfigFile(){
  while (!userInput){
    RGB_write(purple);
  }
  RGB_write(off);
  userInput = false;
  Serial.println("\nOpen config file to write...");
  File cfg_file = SD.open("/config.txt", FILE_WRITE);

  if (!cfg_file) {
    Serial.println("\nCAN'T OPEN CONFIG FILE !");
    return false;
  }
  
  cfg_file.println(inputMessage);
  cfg_file.close();
  Serial.println("\nSUCCESS setup config file...");
  return true;
}

void setBoxID(){
  File myFile = SD.open("/box_id_file.txt", FILE_READ);
  boxID = myFile.readStringUntil('\n');
  boxID.remove(boxID.length() - 1, 1);
  myFile.close();
}

void setup(void){

  Serial.begin(115200);
  while (!Serial) delay(10);

  btnPWD1.setType(LOW_PULL);
  btnPWD1.setTimeout(15000);
  
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  RGB_write(rgb_on);

  // SD card setup
  if(!SD.begin(SD_SS)){
    Serial.println("Card Mount Failed");
    while (!SD.begin(SD_SS)) {
      RGB_error_sd();
      delay(500);
    }
    RGB_write(rgb_on);
  }
  Serial.println("SD Card Mounted");

  if (!SD.exists("/id.txt")) {
    File myFile = SD.open("/id.txt", FILE_WRITE);
    myFile.close();
  }
  
  // nfc setup
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Didn't find PN53x board");
    while (!versiondata){
      versiondata = nfc.getFirmwareVersion();
      RGB_error_nfc();
      delay(500);
    }
    RGB_write(rgb_on);
  }
  Serial.println("PN53x board");

  //config setup
  if (!file_found("/config.txt")) doHardReset();
  RGB_write(rgb_on);

  //box_id_file setup
  if(!file_found("/box_id_file.txt")){
    Serial.println("box_id_file doesn't exist");
    setBoxIdFile();
    while (!file_found("/box_id_file.txt")) {
      setBoxIdFile();
      RGB_error();
      delay(500);
    }
    RGB_write(rgb_on);
  }
  Serial.println("box_id_file exist");

  if (boxID == ""){
    setBoxID();
    while (boxID == ""){
      setBoxID();
      RGB_error();
      delay(500);
    }
    RGB_write(rgb_on);
  }
  Serial.print("box_id: ");
  Serial.println(boxID);
  RGB_write(rgb_on);

  // boxID = "206";

  //rtc setup
  RTC.begin();
  setTime();
  
  RGB_write(off);
  Serial.println("SUCCESS BOX SETUP");
  activeTime = millis();
}

void setTimeOnESP() {

  timeClient.begin();
  timeClient.setTimeOffset(0);

  if(!timeClient.update()){
    Serial.println("Failed to obtain time");
    return;
  }
  
  RTC.settimeUnix(timeClient.getEpochTime());
  Serial.print("after set RTC.gettimeUnix = ");
  Serial.println(RTC.gettimeUnix());
}

void setTime(){
  
  Serial.println("Configuring time...");
  
  // Wifi settings
  WiFiMulti wifiMulti;
  network_config(wifiMulti); //пишем все ssid и пароли из конфига
  wifi_connecting(wifiMulti);//и подключаемся

  if (wifiMulti.run() == WL_CONNECTED){
    setTimeOnESP();
  }
}

void checkTimeForSleeping(){
  unsigned long currentTime = millis();
  if (currentTime - activeTime > 60000){
    activeTime = millis();
    Serial.println("SLEEP");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_2, 1);
    esp_light_sleep_start();
  }
}

void checkSingleClick(){
  if (btnPWD1.isSingle()) {
    activeTime = millis();
    Serial.println("btnPWD1.isSingle()");
    if(readNFC()) sendDataToWIFI();
  }
}

void checkDoubleClick(){
  if (btnPWD1.isDouble()) {
    activeTime = millis();
    Serial.println("btnPWD1.isDouble()");
    sendDataFromSD();
  }
}

void checkTripleClick(){
  if (btnPWD1.isTriple()) {
    activeTime = millis();
    Serial.println("btnPWD1.isTriple()");
    show_charge(get_voltage(1), 730, 675, 642);
  }
}

void checkLongClick(){
  if (btnPWD1.isHolded()){
    RGB_write(blue);
    Serial.println("PRESS BUTTON 3 TIMES TO hard reset");
    unsigned long currentTime = millis();
    bool configIsChanged = false;
    while (millis() - currentTime < 10000){
      btnPWD1.tick();
      if (btnPWD1.isTriple()) {
        configIsChanged = true;
        doHardReset();
      }
    }
    RGB_write(off);
    if (!configIsChanged) Serial.println("DO NOT HARD RESET");
  }
}

void loop(void) {
  btnPWD1.tick();
  checkTimeForSleeping();
  checkSingleClick();
  checkDoubleClick();
  checkTripleClick();
  checkLongClick();
}

void doHardReset(){
  if(SD.exists("/config.txt")){
    Serial.println("\nDelete config file ...");
    SD.remove("/config.txt");
  }
  RGB_write(purple);
  startAccessPoint();
  if (setConfigFile()) Serial.println("SUCCESS setConfigFile");
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void network_config(WiFiMulti &wifiMulti){
  Serial.println("\nOpen config file to read...");
  File cfg_file = SD.open("/config.txt");

  if (!cfg_file){
    Serial.println("\nCan`t open config file");
  }

  while (cfg_file.available()) {
    String cfg_str = cfg_file.readStringUntil('\n');
    int space_index = cfg_str.indexOf(";");
    String ssid = cfg_str.substring(0,space_index);
    String password = cfg_str.substring(space_index + 1);
    const char * c_ssid = ssid.c_str();
    const char * c_password = password.c_str();
    Serial.println(ssid);
    Serial.println(password);
    wifiMulti.addAP(c_ssid, c_password);
  }
  cfg_file.close();
}

bool file_found(const String fileName) {
  if(!SD.exists(fileName)) {
    Serial.println();
    Serial.print(fileName);
    Serial.println(" FILE IS NOT FOUND!");
    return false;
  }
  return true;
}

void startAccessPoint(){
  Serial.println();
  Serial.println("Configuring access point...");
  
  if (!WiFi.softAP(esp32_wifi_ssid, esp32_wifi_password)) {
    Serial.println("ESP32 soft AP creation failed.");
    while (!WiFi.softAP(esp32_wifi_ssid, esp32_wifi_password)){
      RGB_error();
      delay(500);
    }
    RGB_write(rgb_on);
  }

  Serial.println("ESP32 soft AP creation SUCCESS");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server_setup();
}

void sendDataToSD(String fileName, String data, bool ledOn){
  File myFile = SD.open(fileName, FILE_APPEND);
  if (myFile) {
    myFile.println(data);
    Serial.println(data + " send to " + fileName);
    #if MODE == DEBUG_OFF
    if (ledOn){
      RGB_success();
    }
    #endif
  }
  else {
    Serial.println("Can`t open file " + fileName);
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

bool sendToWifi(HTTPClient &https, String data, bool ledOn){
  int httpsResponseCode = https.POST(data);
  Serial.println("data:");
  Serial.println(data);
  Serial.println(httpsResponseCode);
  if (httpsResponseCode == 200){
    if (ledOn){
      RGB_success();
    }
    return true;
  } else {
    #if MODE == DEBUG_ON
    if (ledOn){
      RGB_error();
    }
    #endif
    Serial.println("error wifi sending");
    return false;
  }
}

void wifi_connecting(WiFiMulti &wifiMulti) {
  Serial.println("Connecting Wifi...");
  wifiMulti.run();
  Serial.println("Wifi run");
  Serial.print("WiFi.status() = ");
  Serial.println(WiFi.status());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
}

void sendDataFromSD(){
  RGB_write(yellow);
  activeTime = millis();
  int failSendCount = 0;

  // Wifi settings
  WiFiMulti wifiMulti;
  network_config(wifiMulti); //пишем все ssid и пароли из конфига
  wifi_connecting(wifiMulti);//и подключаемся

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client){
    client->setCACert(rootCACertificate);
    HTTPClient https;
    https.begin(*client, serverName);
    https.addHeader("Content-Type", "application/json");
    RGB_write(yellow);
    activeTime = millis();
    // Send data from SD to wifi
    File myFile = SD.open("/id.txt", FILE_READ);
    if (myFile){
      Serial.println("/id.txt:");
      while (myFile.available()){
        activeTime = millis();
        String buffer = myFile.readStringUntil('\n');      // Считываем с карты весь дотекст в строку до 
                                                            // символа окончания + перевод каретки (без удаления строки)
        if(!sendToWifi(https, buffer, 0)){
          RGB_write(yellow);
          buffer.trim();
          sendDataToSD("/id2.txt", buffer, 0);
          RGB_write(yellow);
          failSendCount++;
        }
        RGB_write(yellow);
      }
    } else {
      Serial.println("error opening /id.txt");
    }
    SD.remove("/id.txt");
    renameFile();
    myFile.close();
    Serial.print("failSendCount = ");
    Serial.println(failSendCount);
    https.end();
    RGB_write(off);
  }
}

void sendDataToWIFI(){
  RGB_write(yellow);
  activeTime = millis();
  int failSendCount = 0;

  // Wifi settings
  WiFiMulti wifiMulti;
  network_config(wifiMulti); //пишем все ssid и пароли из конфига
  wifi_connecting(wifiMulti);//и подключаемся

  WiFiClientSecure *client = new WiFiClientSecure;
  if(client){
    client -> setCACert(rootCACertificate);
    HTTPClient https;
    https.begin(*client, serverName);
    https.addHeader("Content-Type", "application/json");
    if(!sendToWifi(https, result, 1)){
      RGB_write(yellow);
      sendDataToSD("/id.txt", result, 1);
      RGB_write(yellow);
      failSendCount++;
      Serial.print("failSendCount = ");
      Serial.println(failSendCount);
      https.end();
      RGB_write(off);
      return;
    }
    RGB_write(yellow);
    activeTime = millis();
    // Send data from SD to wifi
    File myFile = SD.open("/id.txt", FILE_READ);
    if (myFile){
      Serial.println("/id.txt:");
      while (myFile.available()){
        activeTime = millis();
        String buffer = myFile.readStringUntil('\n');      // Считываем с карты весь дотекст в строку до 
                                                            // символа окончания + перевод каретки (без удаления строки)
        if(!sendToWifi(https, buffer, 0)){
          RGB_write(yellow);
          buffer.trim();
          sendDataToSD("/id2.txt", buffer, 0);
          RGB_write(yellow);
          failSendCount++;
        }
        RGB_write(yellow);
      }
      SD.remove("/id.txt");
      renameFile();
    } else {
      Serial.println("error opening /id.txt");
    }
    myFile.close();
    Serial.print("failSendCount = ");
    Serial.println(failSendCount);
    https.end();
    RGB_write(off);
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
      uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

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
          if (success){
            printHexCharAsOneLine(data, 16);
            if (i == 4) read_data += tagData.substring(tagData.length() - 5, tagData.length());
            if (i == 5) read_data += tagData.substring(0, 4);
            tagData = "";
            nfc.PrintHexChar(data, 16);
          }
          else
          {
            Serial.println("Ooops ... unable to read the requested block.  Try another key?");
            RGB_error();
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
        result += String(RTC.gettimeUnix());
        result += "\", \"secret_key\":\"";
        result += secret_key;
        result += "\"}";
        // {"box_id":"asdfv", "mark_id":"444444444", "event_time":"123123"}
        Serial.println(result);
        RGB_success();
        return true;
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
        RGB_error();
        return false;
      }
    } else{
      Serial.println("This doesn't seem to be an Mifare Classic tag (UUID length != 4 bytes)!");
      RGB_error();
      return false;
    }
  } else {
    Serial.println("cant read tag");
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
     Serial.print("VOLTAGE: analogPin: ");
     Serial.print(voltage);
     Serial.print(" Real: ");
     voltage = (25306+100*voltage)/323;
     Serial.println(voltage);
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

void RGB_error_sd(){ // 4 коротких
  RGB_write(red);
  delay(200);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(200);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(200);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(200);
  RGB_write(off);
  
}

void RGB_error_gsm(){ // 2 длинных
  RGB_write(red);
  delay(1000);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(1000);
  RGB_write(off);
}

void RGB_error_nfc(){//2 коротких, 2 длинных
  RGB_write(red);
  delay(200);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(200);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(1000);
  RGB_write(off);
  delay(200);
  RGB_write(red);
  delay(1000);
  RGB_write(off);
}
#define RXD2 16
#define TXD2 17

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

String boxID                    = "";
String secret_key               = "a086d0ee0aff004b5034fcdb04ec400c";
String serverName               = "http://185.241.68.155:8001/send_data";
String boxActivateServerName    = "http://185.241.68.155:8001/boxes/activate";

void sim_card_setup(){
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  int simCardFail = 0;
  String RespCodeStr = "";
  do{
    if (simCardFail > 2){
      // RGB_error();
    }
    Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
    updateSerial();
    Serial2.println("AT+CIICR");
    updateSerial();
    if (simCardFail > 2){
      // RGB_error();
    }
    Serial2.println("AT+CREG?");
    delay(1500);
    RespCodeStr = "";
    while (Serial2.available()>0) {
      RespCodeStr += char(Serial2.read());
    }
    Serial.print("RespCodeStr = ");
    Serial.println(RespCodeStr);
    simCardFail++;
  } while (!(RespCodeStr.indexOf("+CREG") >= 0));
}

bool setBoxIdFile(){
  
  Serial.println("Configuring box id...");

  Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  updateSerial();
  Serial2.println("AT+CIICR");
  updateSerial();
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial2.println("AT+TERMHTTP");// Send data request to the server
  updateSerial();
  Serial2.println("AT+INITHTTP"); //The basic adhere network command of Internet connection
  updateSerial();
  Serial2.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");//Activate PDP; Internet connection is available after successful PDP activation
  updateSerial();
  Serial2.println("AT+HTTPPOST=\"URL\",\"http://185.241.68.155:8001/boxes/activate\",\"{\"secret_key\":\"a086d0ee0aff004b5034fcdb04ec400c\"}\"");//Get local IP address
  updateSerial();


  String RespCodeStr = "";
  while (Serial2.available()>0) {
    RespCodeStr += char(Serial2.read());
  }
  Serial.print("RespCodeStr = ");
  Serial.println(RespCodeStr);
  Serial.println("END OF RespCodeStr");

  Serial2.println("AT+TERMHTTP"); //The basic adhere network command of Internet connection
  updateSerial();

  return true;
}

void doBoxIdFile(){
  setBoxIdFile();
}

void setup(void){
  Serial.begin(115200);
  while (!Serial) delay(10); 

  sim_card_setup();
  Serial.println("SUCCESS BOX SETUP");
  doBoxIdFile();
}

void updateSerial()
{
  delay(1500);
  while(Serial2.available()) {
    Serial.write(Serial2.read());//Data received by Serial2 will be outputted by Serial}
  }
}

bool sendToGSM(String data, bool ledOn){
  // Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  Serial2.println("AT+CGACT=1,1"); // Activate net
  updateSerial();
  Serial2.println("AT+CIICR");
  updateSerial();
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial2.println("AT+HTTPINIT"); //The basic adhere network command of Internet connection
  updateSerial();
  Serial2.println("AT+HTTPPARA=\"CID\",\"1\"");//Set PDP parameter
  updateSerial();
  Serial2.println("AT+HTTPPARA=\"CONTENT\",\"application/json\"");//Activate PDP; Internet connection is available after successful PDP activation
  updateSerial();
  Serial2.println("AT+HTTPPARA=\"URL\",\"http://185.241.68.155:8001/send_data\"");//Get local IP address
  updateSerial();
  Serial2.println("AT+HTTPDATA");// Connect to the server then the server will send back former data
  updateSerial();
  Serial2.println(data);// Send data request to the server
  delay(1500);
  Serial2.write(26);// Terminator
  delay(1500);
  Serial2.println("AT+HTTPACTION=1");// Send data request to the server
  delay(1500);
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  delay(1500);
  String RespCodeStr = "";
  while (Serial2.available()>0) {
    RespCodeStr += char(Serial2.read());
  }
  Serial.print("RespCodeStr = ");
  Serial.println(RespCodeStr);
  Serial.println("END OF RespCodeStr");

  if (!RespCodeStr.isEmpty() && RespCodeStr.indexOf("200") >= 0){
    if (ledOn){
      // RGB_success();
    }
    Serial.println("SUCCESS gsm sending");
    return true;
  } else {
    #if MODE == DEBUG_ON
    if (ledOn){
      // RGB_error();
    }
    #endif
    Serial.println("error gsm sending");
    return false;
  }
}

void loop(void) {
}
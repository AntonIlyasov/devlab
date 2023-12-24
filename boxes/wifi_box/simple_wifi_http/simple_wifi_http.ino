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

const String secret_key = "a086d0ee0aff004b5034fcdb04ec400c";

//RTC////
iarduino_RTC RTC(RTC_DS1302, 27, 25, 26);


void setup(void){
  Serial.begin(115200);
  while (!Serial) delay(10); 

  //rtc setup
  RTC.begin();
  // setTime();

  Serial.println("SUCCESS BOX SETUP");
}

void updateSerial()
{
  delay(1500);
  while(Serial2.available()) {
    Serial.write(Serial2.read());//Data received by Serial2 will be outputted by Serial}
  }
}

bool sendToGSM(String data, bool ledOn){
  Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
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
  Serial2.println("AT+HTTPPARA=\"URL\",\"http://box-dev.dvlb.ru/app/send_data\"");//Get local IP address
  updateSerial();
  Serial2.println("AT+HTTPDATA");// Connect to the server then the server will send back former data
  updateSerial();
  Serial2.println(data);// Send data request to the server
  delay(2000);
  Serial2.write(26);// Terminator
  delay(2000);
  Serial2.println("AT+HTTPACTION=1");// Send data request to the server
  delay(2000);
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
    if (ledOn){
      // RGB_error();
    }
    Serial.println("error gsm sending");
    return false;
  }
}


void loop(void) {
  String data = "{\"box_id\":\"asdfv\", \"mark_id\":\"111222333\", \"event_time\":\"1700834022";
  data += "\", \"secret_key\":\"";
  data += secret_key;
  data += "\"}";

  sendToGSM(data, 1);
  delay(2000);
}

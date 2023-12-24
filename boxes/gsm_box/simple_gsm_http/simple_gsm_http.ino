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

const String boxID          = "9";
const String secret_key     = "a086d0ee0aff004b5034fcdb04ec400c";
String unixTime             = "";
unsigned long unixTimeLong  = 0;
const uint32_t gmt = 0;
UnixTime timeStamp(gmt);

struct Data_Time{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
};

//RTC////
iarduino_RTC RTC(RTC_DS1302, 27, 25, 26);

void sim_card_setup(){
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  int simCardFail = 0;
  String RespCodeStr = "";
  do{
    if (simCardFail > 2){
      // RGB_error();
    }
    // Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
    Serial2.println("AT+CSTT=\"Public.MC\",\"gdata\",\"gdata\"");// Get IMEI
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

void parseDataTime(Data_Time &curentDataTime, String dataTime){
  int space_index_1 = 0;
  int space_index_2 = dataTime.indexOf("/", space_index_1);
  String year       = dataTime.substring(space_index_1, space_index_2);
  space_index_1     = space_index_2 + 1;
  space_index_2     = dataTime.indexOf("/", space_index_1);
  String month      = dataTime.substring(space_index_1, space_index_2);
  space_index_1     = space_index_2 + 1;
  space_index_2     = dataTime.indexOf(",", space_index_1);
  String day        = dataTime.substring(space_index_1, space_index_2);
  space_index_1     = space_index_2 + 1;
  space_index_2     = dataTime.indexOf(":", space_index_1);
  String hours      = dataTime.substring(space_index_1, space_index_2);
  space_index_1     = space_index_2 + 1;
  space_index_2     = dataTime.indexOf(":", space_index_1);
  String minutes    = dataTime.substring(space_index_1, space_index_2);
  space_index_1     = space_index_2 + 1;
  space_index_2     = dataTime.indexOf("+", space_index_1);
  String seconds    = dataTime.substring(space_index_1, space_index_2);
  
  curentDataTime.year     = strtol(year.c_str(),    NULL, 0);
  curentDataTime.month    = strtol(month.c_str(),   NULL, 0);
  curentDataTime.day      = strtol(day.c_str(),     NULL, 0);
  curentDataTime.hours    = strtol(hours.c_str(),   NULL, 0);
  curentDataTime.minutes  = strtol(minutes.c_str(), NULL, 0);
  curentDataTime.seconds  = strtol(seconds.c_str(), NULL, 0);
}

unsigned long userGetEpochTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hours, uint8_t minutes, uint8_t seconds){
  Serial.println(year);
  Serial.println(month);
  Serial.println(day);
  Serial.println(hours);
  Serial.println(minutes);
  Serial.println(seconds);
  timeStamp.setDateTime(year + 2000, month, day, hours, minutes, seconds); //2017, 1, 1, 10, 4, 22
  Serial.println(timeStamp.getUnix());
  return timeStamp.getUnix();
}

void setTimeOnESP(String dataTime) {
  Data_Time currentDataTime;
  parseDataTime(currentDataTime, dataTime);
  unixTimeLong = userGetEpochTime(currentDataTime.year,
                                  currentDataTime.month,
                                  currentDataTime.day,
                                  currentDataTime.hours,
                                  currentDataTime.minutes,
                                  currentDataTime.seconds);
  Serial.print("!!!!!!!!!unixTimeLong = ");
  Serial.println(unixTimeLong); 
  RTC.settimeUnix(unixTimeLong);
  Serial.print("!!!!!!!!! RTC.gettimeUnix(): ");
  Serial.println(RTC.gettimeUnix());
}

void parseToTime(String input){
  int indexUnixTime = input.length();
  unixTime = input.substring(indexUnixTime-13, indexUnixTime);
  Serial.print("indexUnixTime = ");
  Serial.println(indexUnixTime);
  Serial.print("unixTime = ");
  Serial.println(unixTime);
  unixTimeLong = strtoul(unixTime.c_str(), NULL, 10);
  Serial.print("unixTimeLong = ");
  Serial.println(unixTimeLong); 
  RTC.settimeUnix(unixTimeLong);
}

void setTimeFromAt(){
  Serial.println("Configuring time...");
  Serial2.println("AT+CCLK?");
  delay(1500);

  String RespCodeStr = "";
  while (Serial2.available()>0) {
    RespCodeStr += char(Serial2.read());
  }
  Serial.print("RespCodeStr = ");
  Serial.println(RespCodeStr);

  String clockString = "";
  if (!RespCodeStr.isEmpty() && RespCodeStr.indexOf("+CCLK:") >= 0){
    int x = RespCodeStr.indexOf(String('"')) + 1;   // Find the first occurance of an open quotation.  This is where we begin to read from
    int y = RespCodeStr.lastIndexOf(String('"')); // Find the last occurance of an open quotation. This is where we end.
    clockString = RespCodeStr.substring(x,y);
    Serial.print("clockString = ");
    Serial.println(clockString);
  }
  if (!clockString.isEmpty()){
    setTimeOnESP(clockString);
  }
}

void setTime(){

  Serial.println("Configuring time...");
  // Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  Serial2.println("AT+CSTT=\"Public.MC\",\"gdata\",\"gdata\"");// Get IMEI
  updateSerial();
  Serial2.println("AT+CIICR");
  updateSerial();
  Serial2.println("AT+CGACT=1,1");
  updateSerial();
  Serial2.println("AT+TERMHTTP");// Send data request to the server
  updateSerial();
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial2.println("AT+INITHTTP"); //The basic adhere network command of Internet connection
  updateSerial();
  Serial2.println("AT+HTTPGET=\"http://box-dev.dvlb.ru/app/timeUnix\"");// Connect to the server then the server will send back former data
  delay(10000);

  String RespCodeStr = "";
  while (Serial2.available()>0) {
    RespCodeStr += char(Serial2.read());
  }
  Serial.print("RespCodeStr = ");
  Serial.println(RespCodeStr);
  Serial.println("EndRespCodeStr");

  parseToTime(RespCodeStr);

  Serial2.println("AT+TERMHTTP");// Send data request to the server
  delay(1500);
}

void setup(void){
  Serial.begin(115200);
  while (!Serial) delay(10); 

  sim_card_setup();

  //rtc setup
  RTC.begin();
  setTimeFromAt();

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
  // Serial2.println("AT+CSTT=\"internet.mts.ru\",\"mts\",\"mts\"");// Get IMEI
  Serial2.println("AT+CSTT=\"Public.MC\",\"gdata\",\"gdata\"");// Get IMEI
  updateSerial();
  Serial2.println("AT+CIICR");
  updateSerial();
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  updateSerial();
  Serial2.println("AT+TERMHTTP");// Send data request to the server
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
  delay(5000);
  Serial2.write(26);// Terminator
  delay(3000);
  Serial2.println("AT+HTTPACTION=1");// Send data request to the server
  delay(3000);
  Serial2.println("AT+HTTPTERM");// Send data request to the server
  delay(3000);
  String RespCodeStr = "";
  while (Serial2.available()>0) {
    RespCodeStr += char(Serial2.read());
  }
  Serial.println(RespCodeStr);
  if (!RespCodeStr.isEmpty() && RespCodeStr.indexOf("200") >= 0){
    Serial.println("SUCCESS gsm sending");
    return true;
  } else {
    Serial.println("error gsm sending");
    return false;
  }
}

void loop(void) {

  String result = "";
  result = "{\"box_id\":\"";
  result += boxID;
  result += "\", \"mark_id\":\"";
  result += "111222333";
  result += "\", \"event_time\":\"";
  result += String(RTC.gettimeUnix());
  result += "\", \"secret_key\":\"";
  result += secret_key;
  result += "\"}";
  Serial.print("result: ");
  Serial.println(result);
  Serial.print("RTC.gettimeUnix(): ");
  Serial.println(RTC.gettimeUnix());
  sendToGSM(result, 1);
  delay(2000);
}

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Arduino_JSON.h>
#include "FS.h"
#include <SD.h>

WiFiMulti WiFiMulti;

const char * c_ssid     = "Keenetic-0154";
const char * c_password = "123456777";

//WiFi////
String unixTime                 = "";
String secret_key               = "a086d0ee0aff004b5034fcdb04ec400c";
String serverName               = "http://185.241.68.155:8001/send_data";
String boxActivateServerName    = "http://185.241.68.155:8001/boxes/activate";
String timeServerName           = "http://worldtimeapi.org/api/timezone/Europe/London";

bool setBoxIdFile(){

  WiFiClient client;
  HTTPClient http;
  http.begin(client, timeServerName);            // Your Domain name with URL path or IP address with path

  int httpResponseCode = http.GET();

  String payload = "{}"; 

  if (httpResponseCode == 200){
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    JSONVar myObject = JSON.parse(payload);
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return false;
    }
    Serial.print("JSON object = ");
    Serial.println(myObject);
    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();
    JSONVar value = myObject[keys[11]];
    Serial.print(keys[11]);
    Serial.print(" = ");
    Serial.println(value);
    unixTime = String(JSON.stringify(value));
    Serial.print("unixTime: ");
    Serial.println(unixTime);
    // Free resources
    http.end();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    // Free resources
    http.end();
    return false;
  }

  return true;
}

void doBoxIdFile(){
  setBoxIdFile();
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(c_ssid, c_password);

  Serial.println();
  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  doBoxIdFile();
}

void loop() {
}

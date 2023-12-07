#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Arduino_JSON.h>
#include "FS.h"
#include <SD.h>

WiFiMulti WiFiMulti;

const char * c_ssid     = "aquadisco";
const char * c_password = "polytech428";

//WiFi////
String boxID                    = "";
String secret_key               = "a086d0ee0aff004b5034fcdb04ec400c";
String serverName               = "http://185.241.68.155:8001/send_data";
String boxActivateServerName    = "http://185.241.68.155:8001/boxes/activate";

bool setBoxIdFile(){

  WiFiClient client;
  HTTPClient http;
  http.begin(client, boxActivateServerName);            // Your Domain name with URL path or IP address with path
  http.addHeader("Content-Type", "application/json");   // Specify content-type header
  
  String result = "";
  result = "{\"secret_key\":\"";
  result += secret_key;
  result += "\"}";
  Serial.println(result);

  int httpResponseCode = http.POST(result);
  Serial.println(httpResponseCode);

  String payload = "{}"; 

  if (httpResponseCode == 200){
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
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
    // Free resources
    http.end();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    // Free resources
    http.end();
    return false;
  }
  
  Serial.println("\nOpen box_id_file file to write...");
  File box_id_file = SD.open("/box_id_file.txt", FILE_WRITE);

  if (!box_id_file) {
    Serial.println("\nCAN'T OPEN box_id_file FILE !");
    return false;
  }
  
  box_id_file.println(boxID);
  box_id_file.close();

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

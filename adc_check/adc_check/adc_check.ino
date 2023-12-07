#include <WiFi.h>
#include <WiFiMulti.h>

#define PIN 27

WiFiMulti WiFiMulti;

const char * c_ssid     = "Keenetic-0154";
const char * c_password = "uhBkDPUJ";

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(PIN, OUTPUT);

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

  delay(500);
}

void loop() {
  digitalWrite(PIN, HIGH);
}

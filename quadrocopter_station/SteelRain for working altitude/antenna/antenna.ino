#include <ServoSmooth.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define PI 3.1415926535897932384626433832795

int count = 0;

ServoSmooth servoHor;
ServoSmooth servoVert;
static const int RXPin = 7, TXPin = 8;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// TCP сервер, возвращает полученные данные
#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

#include <QMC5883LCompass.h>

byte mac[] = {0x91,0x46,0x45,0x4D,0x9A,0x04}; // MAC-адрес
byte ip[] = {192, 168, 0, 15}; // IP-адрес

EthernetServer server(11000); // создаем сервер, порт 2000
EthernetClient client; // объект клиент
boolean clientAlreadyConnected= false; // признак клиент уже подключен

int horAngle = 0;
int vertAngle = 45;
bool stateHor;
bool stateVert;
double stationLat = 60.0287062;
double stationLon = 30.2554687;
double latLength = 111134.861111;
double lonLength = 0.0;
long R = 6371000;
double stationAlt = 5.0;
double quadroPosition[3] = {0.0,0.0,0.0};

void calculateLength(double lon){
  lonLength = (PI*R*cos(lon))/180;
  }

int counter = 0;
QMC5883LCompass compass;

void setup()
{

  Serial.begin(115200);
  
  ss.begin(GPSBaud);
  Ethernet.begin(mac,ip);
  if(Ethernet.linkStatus() == LinkOFF){
  Serial.println("no cabel");
  }
  server.begin(); // включаем ожидание входящих соединений;
  Serial.print("Server address:");
  Serial.println(Ethernet.localIP()); // выводим IP-адрес контроллера
  
  while(1){
  
    client = server.available(); // ожидаем объект клиент
    if (client) {
      // есть данные от клиента
      if (clientAlreadyConnected == false) {
        // сообщение о подключении
        Serial.println("Client connected");
        Serial.println("Server ready"); // ответ клиенту
        clientAlreadyConnected= true;
      }
      
      if(client.available() > 0) {
        double chr = client.parseFloat();
        Serial.print(chr,8);
        Serial.print("\t");
        quadroPosition[counter] = chr;
        counter++;
        if (counter == 3){
          //calculateAngles(quadroPosition[0], quadroPosition[1], quadroPosition[2]);
          Serial.println();
          counter = 0;
        }
        Serial.println(F("quadroPosition:"));
        Serial.print(F("X = "));
        Serial.println(quadroPosition[0]);
        Serial.print(F("Y = "));
        Serial.println(quadroPosition[1]);
        Serial.print(F("Z = "));
        Serial.println(quadroPosition[2]);
        count++;
        Serial.print(F("count = "));
        Serial.println(count);
        
      }
    }
  }
}


void loop() {}

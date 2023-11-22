#define DEBUG_ON    1
#define DEBUG_OFF   0
#define MODE        DEBUG_ON

#include <Servo.h>
#include <TinyGPSPlus.h>
#include <QMC5883LCompass.h>
#include <SPI.h>
#include <Ethernet.h>

#define PI 3.1415926535897932384626433832795
#define RAD_EARTH 6372795

Servo servoHor;
Servo servoVert;

int vertAngle = 90;
int horAngle  = 90;

#define USE_TIMER_2     true
#include "TimerInterrupt.h"

#if USE_TIMER_2
void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  servoHor.write(horAngle);
  servoVert.write(vertAngle);
}
#endif

unsigned int outputPin1 = LED_BUILTIN;
#define TIMER1_INTERVAL_MS    50
#define TIMER1_DURATION_MS    0

EthernetServer server(11000);                         // создаем сервер, порт 2000
EthernetClient client;                                // объект клиент
TinyGPSPlus gps;
QMC5883LCompass compass;

unsigned long start       = millis();
unsigned long frq_arr[3]  = {0};

#if MODE == DEBUG_ON
  float station_latitude   = 0;
  float station_longitude  = 0;
#endif

#if MODE == DEBUG_OFF
  long station_latitude   = 0;
  long station_longitude  = 0;
#endif

long station_azimuth    = 0;
int servo_angles[2]     = {0};

float magnetic_declination  = 0;
float magnPol_latitude      = 86.2;
float magnPol_longitude     = 170.4;

float northPol_latitude   = 90.0;
float northPol_longitude  = 0.0;
float lat_0               = 0.0;
float long_0              = 0.0;
float long_15             = 15.0;

//до сюда

void compass_setup() {
#if MODE == DEBUG_ON
  Serial.println(F("Compass initialisation ..."));
#endif
  compass.init();
  compass.setSmoothing(10, true);
compass.setCalibrationOffsets(-16.00, -337.00, -540.00);
compass.setCalibrationScales(1.03, 0.95, 1.02);
#if MODE == DEBUG_ON
  Serial.println(F("Compass SUCCESS ..."));
#endif
}

/* Время между вызовами данной функции, а также частота вызова данной функции*/
void time_check() {
  unsigned long time = millis() - start;
  start = millis();
#if MODE == DEBUG_ON
  Serial.print(F("\nTIME: "));
  Serial.println(time);
#endif
  unsigned long frq_cur = 1000 / time;
  frq_arr[2] = frq_arr[1];
  frq_arr[1] = frq_arr[0];
  frq_arr[0] = frq_cur;

  unsigned long frq = 0;
  for (int i = 0; i<3; i++) {
    frq += frq_arr[i];
  }
  frq = frq/3;
#if MODE == DEBUG_ON
  Serial.print(F("FRQ: "));
  Serial.println(frq);
#endif
}

void ethernet_setup(){
#if MODE == DEBUG_ON
  Serial.println(F("Ethernet initialisation ..."));
#endif
  byte mac[]      = {0x91,0x46,0x45,0x4D,0x9A,0x04};    // MAC-адрес
  byte ip[]       = {192, 168, 0, 20};                   // IP-адрес
  Ethernet.begin(mac,ip);

  while(Ethernet.linkStatus() == LinkOFF){
#if MODE == DEBUG_ON
    Serial.println(F("no cabel"));
#endif
  }

  server.begin();                                       // включаем ожидание входящих соединений;
#if MODE == DEBUG_ON
  Serial.print(F("Server address:"));
  Serial.println(Ethernet.localIP());                   // выводим IP-адрес контроллера
  Serial.println(F("Waiting for client"));
#endif

  client = server.available();
  while(!client){
    client = server.available();
    delay(1);
  }

  while (client.available() > 0) {
    client.read();
  }

#if MODE == DEBUG_ON
  Serial.println(F("Ethernet SUCCESS ..."));
#endif
}

void gps_setup(){
  while (!station_latitude || !station_longitude){
    if (Serial.available() > 0 && gps.encode(Serial.read())){
      get_station_location();
    }
  }
  Serial.end();
}

void get_station_location(){
  if (gps.location.isValid()){
    station_latitude  = gps.location.lat();
    station_longitude = gps.location.lng();
  }
}

void servo_setup(){
  servoHor.attach(3, 500, 2500);
  servoVert.attach(6, 500, 2500);
  servoHor.write(horAngle);
  servoVert.write(vertAngle);
#if MODE == DEBUG_ON
  Serial.println(F("SERVO SUCCESS"));
#endif
}

void long_to_bytes(long &val, uint8_t *bytes) {
#if MODE == DEBUG_ON
  Serial.print(F("OUTPUT = "));
  Serial.println(val);
#endif

  bytes[0] = val & 0xFF;                /*0-7 bits*/
  bytes[1] = (val & 0xFF00) >> 8;       /*8-15 bits*/
  bytes[2] = (val & 0xFF0000) >> 16;    /*16-23 bits*/
  bytes[3] = (val & 0xFF000000) >> 24;  /*24-31 bits*/

#if MODE == DEBUG_ON
  Serial.print(F("bytes: "));
  Serial.print(bytes[0]);
  Serial.print(F(" "));
  Serial.print(bytes[1]);
  Serial.print(F(" "));
  Serial.print(bytes[2]);
  Serial.print(F(" "));
  Serial.print(bytes[3]);
  Serial.print(F(" "));
  Serial.println("");
#endif
}

void send_station_localization_2_quadro(){
#if MODE == DEBUG_ON
  Serial.println(F("sending data to tcp"));
#endif

  String data_to_send = String(station_latitude) + " " + String(station_longitude) + " " + String(station_azimuth);
  server.write(data_to_send.c_str(), data_to_send.length());

#if MODE == DEBUG_ON
    Serial.println(data_to_send);
    Serial.println(F("SUCCESS SEND DATA TO TCP"));
#endif
}

double radToDeg(double rad){
  return rad*180/PI;
}

void setup()
{
#if MODE == DEBUG_ON
  Serial.begin(115200);
#else
  Serial.begin(9600);
#endif

  compass_setup();
  get_compass_data(1);
  servo_setup();

#if MODE == DEBUG_OFF
  gps_setup();
#else
  station_latitude  = 60.028729;
  station_longitude = 30.257502;
  Serial.println(F("here1"));
  Serial.print(F("station_latitude = "));
  Serial.println(station_latitude, 6);
  Serial.print(F("station_longitude = "));
  Serial.println(station_longitude, 6);
#endif

  // float dist_northPol_magnPol = gps.distanceBetween(77.1539, -139.398, -77.1804, -139.55);
  // float dist_magnPol_station  = gps.distanceBetween(77.1539, 120.398, 77.1804, 129.55);
  // float dist_northPol_station = gps.distanceBetween(77.1539, -120.398, 77.1804, 129.55);

  // float dist_northPol_magnPol = gps.distanceBetween(northPol_latitude, northPol_longitude, magnPol_latitude, magnPol_longitude);//считает верно
  // float dist_magnPol_station  = gps.distanceBetween(magnPol_latitude, magnPol_longitude, station_latitude, station_longitude);  //считает верно
  // float dist_northPol_station = gps.distanceBetween(northPol_latitude, northPol_longitude, station_latitude, station_longitude);//считает верно

float northPol_latitude   = 90.0;
float northPol_longitude  = 0.0;
float lat_0               = 0.0;
float long_0              = 0.0;
float long_15             = 15.0;

  float dist_northPol_00 = gps.distanceBetween(northPol_latitude, northPol_longitude, lat_0, long_0);
  float dist_00_15       = gps.distanceBetween(lat_0, long_0, lat_0, long_15);  
  float dist_northPol_15 = gps.distanceBetween(northPol_latitude, northPol_longitude, lat_0, long_15);

  // float dist_northPol_magnPol_rad = dist_northPol_magnPol / RAD_EARTH;  //дистанция(длина дуги) на радиус земли
  // float dist_magnPol_station_rad  = dist_magnPol_station  / RAD_EARTH;
  // float dist_northPol_station_rad = dist_northPol_station / RAD_EARTH;

  float dist_northPol_00_rad = dist_northPol_00 / RAD_EARTH;  //дистанция(длина дуги) на радиус земли
  float dist_00_15_rad  = dist_00_15  / RAD_EARTH;
  float dist_northPol_15_rad = dist_northPol_15 / RAD_EARTH;

  // float a = dist_northPol_magnPol_rad;
  // float b = dist_magnPol_station_rad;
  // float c = dist_northPol_station_rad;

  float c = dist_northPol_00_rad;
  float b = dist_00_15_rad;
  float a = dist_northPol_15_rad;

  float chislitel   = cos(a) - cos(b)*cos(c);
  float znamenatel  = sin(b)*sin(c);

  float cos_magnetic_declination = acos(chislitel/znamenatel);

  magnetic_declination = radToDeg(acos(cos_magnetic_declination));
  station_azimuth += magnetic_declination;

#if MODE == DEBUG_ON
  Serial.println(F("############################"));
  Serial.println(F("here2"));
  Serial.print(F("station_latitude = "));
  Serial.println(station_latitude, 6);
  Serial.print(F("station_longitude = "));
  Serial.println(station_longitude, 6);
  
  // Serial.println(F("############################"));
  // Serial.println(F("dist_northPol_magnPol = "));
  // Serial.println(dist_northPol_magnPol);
  // Serial.println(F("dist_magnPol_station = "));
  // Serial.println(dist_magnPol_station);
  // Serial.println(F("dist_northPol_station = "));
  // Serial.println(dist_northPol_station);

  Serial.println(F("############################"));

  Serial.println(F("a = "));
  Serial.println(a, 6);
  Serial.println(F("b = "));
  Serial.println(b, 6);
  Serial.println(F("c = "));
  Serial.println(c, 6);

  Serial.println(F("############################"));

  Serial.println(F("cos_magnetic_declination = "));
  Serial.println(cos_magnetic_declination);
  Serial.println(F("magnetic_declination = "));
  Serial.println(magnetic_declination);
  Serial.println(F("station_azimuth = "));
  Serial.println(station_azimuth);
  Serial.println(F("############################"));
#endif

  ethernet_setup();
  send_station_localization_2_quadro();

#if MODE == DEBUG_ON
  Serial.println("SUCCESS STATION SETUP");
#endif

#if USE_TIMER_2
  ITimer2.init();
  if (ITimer2.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  {
    
#if MODE == DEBUG_ON
    Serial.print(F("Starting  ITimer1 OK, millis() = "));
    Serial.println(millis());
#endif
  }
  else{
#if MODE == DEBUG_ON
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
#endif
  }

#endif
}

void loop()
{
  get_angles(1);
}

void get_angles(bool debug){
  client = server.available();                        // ожидаем объект клиент
  if(client.available() > 0) {
    for(int i = 0; i < 2; i++){
      int chr = client.parseInt();
      servo_angles[i] = chr;
    }
    // time_check();
    vertAngle = servo_angles[0];
    horAngle  = servo_angles[1];
    horAngle  = constrain(horAngle, 0 ,180);
    vertAngle = constrain(vertAngle, 60, 180);

    if (debug) {
#if MODE == DEBUG_ON
      Serial.println(F("servo_angles:"));
      Serial.print(F("vertAngle = "));
      Serial.println(vertAngle);
      Serial.print(F("horAngle = "));
      Serial.println(horAngle);
#endif
    }
  }
}

void get_compass_data(bool debug) {
  
  int compass_read_count = 0;
  while(compass_read_count < 5) {
    compass_read_count++;
    compass.read();
    delay(250);
  }

  compass.read();
  station_azimuth = compass.getAzimuth();
  station_azimuth = 20;
    
  if (debug) {
#if MODE == DEBUG_ON
    Serial.println("\n========COMPASS DATA========");
    Serial.print(F("station_azimuth = "));
    Serial.println(station_azimuth);
#endif
  }
}

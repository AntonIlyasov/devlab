#define DEBUG_ON    1
#define DEBUG_OFF   0
#define MODE        DEBUG_ON

#include <Servo.h>
#include <TinyGPSPlus.h>
#include <QMC5883LCompass.h>
#include <SPI.h>
#include <Ethernet.h>

Servo servoHor;
Servo servoVert;

int vertAngle = 90;
int horAngle  = 90;

#define USE_TIMER_2     true
#include "TimerInterrupt.h"

#if USE_TIMER_2
void TimerHandler1(unsigned int outputPin = LED_BUILTIN)
{
  // Serial.print(F("servoHor.write(horAngle) = "));
  // Serial.println(horAngle);
  // Serial.print(F("servoVert.write(vertAngle) = "));
  // Serial.println(vertAngle);

  servoHor.write(horAngle);
  servoVert.write(vertAngle);
}
#endif

unsigned int outputPin1 = LED_BUILTIN;
#define TIMER1_INTERVAL_MS    30
#define TIMER1_DURATION_MS    0

// #if MODE == DEBUG_ON
// #elif MODE == DEBUG_OFF
// #endif

#if MODE == DEBUG_ON
  // #include <SoftwareSerial.h>
#endif

EthernetServer server(11000);                         // создаем сервер, порт 2000
EthernetClient client;                                // объект клиент

TinyGPSPlus gps;

#if MODE == DEBUG_ON
  // SoftwareSerial ss(4, 2);                              // RXPin = 4, TXPin = 2;
#endif

QMC5883LCompass compass;
#define PI 3.1415926535897932384626433832795

bool start_session = true;

int count = 0;

double equatorLength = 40075014.172304363;
double latLength     = 111134.861111;
double lonLength     = 0.0;
  
unsigned long start = millis();
unsigned long frq_arr[3] =  {0};

double station_latitude  = 0;
double station_longitude = 0;
double station_altitude  = 0;
double station_azimuth   = 0;

double quadroPosition[3] = {0};
bool   getQuadroPosition = false;

void compass_setup() {
#if MODE == DEBUG_ON
  Serial.println(F("Compass initialisation ..."));
#endif
  compass.init();
  compass.setSmoothing(10, true);
  compass.setCalibrationOffsets(-312.00, -259.00, -214.00);
  compass.setCalibrationScales(1.09, 0.97, 0.95);
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
#endif
  client = server.available();                          // ожидаем объект клиент
  int len = client.available();
  byte buffer[len];
  while(client.available() > 0){
    client.read(buffer, len);
  }
#if MODE == DEBUG_ON
  Serial.println(F("Ethernet SUCCESS ..."));
#endif
}

double radToDeg(double rad){
  return rad*180/PI;
}

double degToRad(double degg){
  return degg*PI/180;
}

void getLonLength(){
  lonLength = equatorLength / 360 * cos(degToRad(station_latitude));
#if MODE == DEBUG_ON
  Serial.print(F("\ngetLonLength return lonLength: "));
  Serial.println(lonLength);
#endif

}

void gps_setup(){

#if MODE == DEBUG_ON
  Serial.println(F("GPS initialisation ..."));
#endif

  while (!station_latitude || !station_longitude){
#if MODE == DEBUG_ON
    // if (ss.available() > 0 && gps.encode(ss.read())){
    //   get_station_location();
    // }
#else 
    if (Serial.available() > 0 && gps.encode(Serial.read())){
      get_station_location();
    }
#endif
  }
#if MODE == DEBUG_ON
  // ss.end();
#else 
  Serial.end();
#endif

#if MODE == DEBUG_ON
  Serial.println(F("GPS SUCCESS ..."));
#endif

}

void get_station_location()
{
  if (gps.location.isValid())
  {
    station_latitude  = gps.location.lat();
    station_longitude = gps.location.lng();
#if MODE == DEBUG_ON
    Serial.print(F("station_latitude = "));
    Serial.println(station_latitude, 6);
    Serial.print(F("station_longitude = "));
    Serial.println(station_longitude, 6);
#endif
  }
  else
  {
#if MODE == DEBUG_ON
    Serial.println(F("INVALID LOCATION"));
#endif
  }
}

void servo_setup(){
  servoHor.attach(3, 500, 2500);
  servoVert.attach(6, 500, 2500);
  servoHor.write(90);       
  servoVert.write(90);
}

void setup()
{
#if MODE == DEBUG_ON
  Serial.begin(115200);
  // ss.begin(9600);
#else
  Serial.begin(9600);
#endif

  compass_setup();
  ethernet_setup();
  servo_setup();
  // gps_setup();
//    //
    station_latitude  = 60.0287092;
    station_longitude = 30.2574947;    
    Serial.print(F("station_latitude = "));
    Serial.println(station_latitude, 6);
    Serial.print(F("station_longitude = "));
    Serial.println(station_longitude, 6);
//    //
  getLonLength();
  get_compass_data(1);
#if MODE == DEBUG_ON
  Serial.println("SUCCESS STATION SETUP");
#endif

#if USE_TIMER_2
  ITimer2.init();
  if (ITimer2.attachInterruptInterval(TIMER1_INTERVAL_MS, TimerHandler1, outputPin1, TIMER1_DURATION_MS))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = "));
    Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
#endif
}

void loop()
{
  get_quadroPosition(1);

  if (getQuadroPosition && start_session && quadroPosition[0] && quadroPosition[1]) {
    station_altitude = quadroPosition[2]; //quadroPosition[2];
    start_session = false;
  }
  if (!start_session){
    follow_quadro();
  }

}

void show_station_pose(){
#if MODE == DEBUG_ON
  // Serial.print(F("station_latitude = "));
  // Serial.println(station_latitude, 6);
  // Serial.print(F("station_longitude = "));
  // Serial.println(station_longitude, 6);
  // Serial.print(F("station_altitude = "));
  // Serial.println(station_altitude, 6);
#endif
}

void get_quadroPosition(bool debug){
  client = server.available();                        // ожидаем объект клиент
  if(client.available() > 0) {
    for(int i = 0; i < 3; i++){
      double chr = client.parseFloat();
      quadroPosition[i] = chr;
    }
    // time_check();
    getQuadroPosition = true;
    count++;
    if (debug) {
#if MODE == DEBUG_ON
      Serial.println(F("quadroPosition:"));
      Serial.print(F("latitude = "));
      Serial.println(quadroPosition[0], 6);
      Serial.print(F("lontitude = "));
      Serial.println(quadroPosition[1], 6);
      Serial.print(F("altitude = "));
      Serial.println(quadroPosition[2], 6);
      // Serial.print(F("count = "));
      // Serial.println(count); 
#endif
    }
  } else {
    getQuadroPosition = false;
  }
}

void follow_quadro(){
  if (!getQuadroPosition) return;
  getQuadroPosition = false;
  calculateAngles(quadroPosition[0], quadroPosition[1], quadroPosition[2]);
}

void calculateAngles(double latitude, double longitude, double alt){  

#if MODE == DEBUG_ON
  Serial.println(F("!!!calculateAngles!!!"));
  Serial.print(F("station_azimuth = "));
  Serial.println(station_azimuth);
  Serial.print(F("latLength = "));
  Serial.println(latLength);
  Serial.print(F("lonLength = "));
  Serial.println(lonLength);
#endif

  float dlat        = (latitude   - station_latitude)  * latLength;     //dy
  float dlon        = (longitude  - station_longitude) * lonLength;     //dx
  float dalt        = alt - station_altitude;                           //dz
  float dist_hor    = sqrt(pow(dlat,2) + pow(dlon,2));
  float dist        = sqrt(pow(dlat,2) + pow(dlon,2) + pow(dalt,2));

#if MODE == DEBUG_ON
  Serial.print(F("dlat = "));
  Serial.println(dlat);
  Serial.print(F("dlon = "));
  Serial.println(dlon);
  Serial.print(F("dalt = "));
  Serial.println(dalt);
  Serial.print(F("dist_hor = "));
  Serial.println(dist_hor);
  Serial.print(F("dist = "));
  Serial.println(dist);
#endif

  vertAngle = 90 + radToDeg(asin(dalt/dist));
  horAngle  = 90 - station_azimuth + radToDeg(asin(dlon/dist_hor));

#if MODE == DEBUG_ON
  Serial.print(F("vertAngle = "));
  Serial.println(vertAngle);
  Serial.print(F("horAngle = "));
  Serial.println(horAngle);
#endif
}

// void go_to_angle(int ver_tar, int hor_tar, int points) {
//   int hor_cur = servoHor.read();
//   int ver_cur = servoVert.read();

//   servoHor.write(hor_tar);
//   servoVert.write(ver_tar);
//   delay(20); 
//   int deltaHor = hor_tar - hor_cur;
//   int deltaVer = ver_tar - ver_cur;
  
//   for (int i = 1; i <= points; i++) {
//     servoHor.write(hor_cur + i*deltaHor/points);
//     servoVert.write(ver_cur + i*deltaVer/points);
//     delay(20/points);
//   }
// }

void get_compass_data(bool debug) {
  
  int compass_read_count = 0;
  while(compass_read_count < 5) {
    compass_read_count++;
    compass.read();
    delay(250);
  }

  compass.read();
  station_azimuth = compass.getAzimuth();
  // station_azimuth = 0;
    
  if (debug) {
#if MODE == DEBUG_ON
    Serial.println("\n========COMPASS DATA========");
    Serial.print(F("station_azimuth = "));
    Serial.println(station_azimuth);
#endif
  }
}

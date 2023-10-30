#define DEBUG_ON 1
#define DEBUG_OFF 0
#define MODE DEBUG_ON

// #if MODE == DEBUG_ON
// #elif MODE == DEBUG_OFF
// #endif

#if MODE == DEBUG_ON
  #include <SoftwareSerial.h>
#endif

#include <ServoSmooth.h>
#include <TinyGPSPlus.h>
#include <QMC5883LCompass.h>
#include <SPI.h>
#include <Ethernet.h>

EthernetServer server(11000);                         // создаем сервер, порт 2000
EthernetClient client;                                // объект клиент
ServoSmooth servoHor;
ServoSmooth servoVert;
TinyGPSPlus gps;
#if MODE == DEBUG_ON
  SoftwareSerial ss(4, 2);                              // RXPin = 4, TXPin = 2;
#endif
QMC5883LCompass compass;
#define PI 3.1415926535897932384626433832795

bool start_session = true;

int count = 0;

double equatorLength = 40075014.172304363;
double latLength     = 111134.861111;
double lonLength     = 0.0;
  
bool stateHor;
bool stateVert;
double vertAngle = 0;
double horAngle  = 0;

unsigned long start = millis();
unsigned long frq_arr[3] =  {0};

double station_latitude  = 0;
double station_longitude = 0;
double station_altitude  = 0;
double station_xyz[3]    = {0};
double station_azimuth   = 0;
double station_peleng    = 0;

double quadroPosition[3] = {0};
bool   getQuadroPosition = false;

void compass_setup() {
#if MODE == DEBUG_ON
  Serial.println(F("Compass initialisation ..."));
#endif
  compass.init();
  compass.setSmoothing(10, true);
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
  byte mac[]      = {0x91,0x46,0x45,0x4D,0x9A,0x04};    // MAC-адрес
  byte ip[]       = {172, 24, 1, 20};                   // IP-адрес
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
}

double radToDeg(double rad){
  return rad*180/PI;
}

double degToRad(double degg){
  return degg*PI/180;
}

void getLonLength(){
  lonLength = equatorLength / 360 * cos(degToRad(station_latitude));
  Serial.print(F("\nequatorLength: "));
  Serial.println(equatorLength);
  Serial.print(F("\ndegToRad(station_latitude): "));
  Serial.println(degToRad(station_latitude));
  Serial.print(F("\nstation_latitude: "));
  Serial.println(station_latitude);
  Serial.print(F("\ngetLonLength return lonLength: "));
  Serial.println(lonLength);
}

void gps_setup(){
  while (!station_latitude || !station_longitude){
#if MODE == DEBUG_ON
    if (ss.available() > 0 && gps.encode(ss.read())){
      get_station_location();
    }
#else 
    if (Serial.available() > 0 && gps.encode(Serial.read())){
      get_station_location();
    }
#endif
  }
  getLonLength();
#if MODE == DEBUG_ON
  ss.end();
#else 
  Serial.end();
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
  servoHor.attach(3, 500, 2500,90);
  servoVert.attach(6, 500, 2500,90);
  servoHor.setSpeed(90);                      // ограничить скорость
  servoHor.setAccel(0.2);
  servoVert.setSpeed(90);                     // ограничить скорость
  servoVert.setAccel(0.2);
}

void setup()
{
#if MODE == DEBUG_ON
  Serial.begin(115200);
  ss.begin(9600);
#else
  Serial.begin(9600);
#endif

  compass_setup();
  ethernet_setup();
//  gps_setup();
 //    //
     station_latitude  = 60.028669;
     station_longitude = 30.257185;    
     Serial.print(F("station_latitude = "));
     Serial.println(station_latitude, 6);
     Serial.print(F("station_longitude = "));
     Serial.println(station_longitude, 6);
     getLonLength();
 //    //
  get_compass_data(1);
#if MODE == DEBUG_ON
  Serial.println("SUCCESS STATION SETUP");
#endif
}

void loop()
{
  get_quadroPosition(1);

  if (getQuadroPosition && start_session) {
    station_altitude = quadroPosition[2];
    show_station_pose();
    start_session = false;
    servo_setup();
  }
  
  if (!start_session){
    stateHor  = servoHor.tick();
    stateVert = servoVert.tick();
  }
  follow_quadro();

}

void show_station_pose(){
#if MODE == DEBUG_ON
  Serial.print(F("station_latitude = "));
  Serial.println(station_latitude, 6);
  Serial.print(F("station_longitude = "));
  Serial.println(station_longitude, 6);
  Serial.print(F("station_altitude = "));
  Serial.println(station_altitude, 6);
#endif
}

void get_quadroPosition(bool debug){
  client = server.available();                        // ожидаем объект клиент
  if(client.available() > 0) {
    for(int i = 0; i < 3; i++){
      double chr = client.parseFloat();
      quadroPosition[i] = chr;
    }
    time_check();
    if (debug) {
#if MODE == DEBUG_ON
      Serial.println(F("quadroPosition:"));
      Serial.print(F("latitude = "));
      Serial.println(quadroPosition[0], 6);
      Serial.print(F("lontitude = "));
      Serial.println(quadroPosition[1], 6);
      Serial.print(F("altitude = "));
      Serial.println(quadroPosition[2], 6);
#endif
    }
    
    getQuadroPosition = true;
    count++;
#if MODE == DEBUG_ON
    Serial.print(F("count = "));
    Serial.println(count); 
#endif
  } else {
    getQuadroPosition = false;
  }
}

void follow_quadro(){
  if (!getQuadroPosition) return;
  getQuadroPosition = false;
  calculateAngles(quadroPosition[0], quadroPosition[1], quadroPosition[2]);
  writeServos(vertAngle, horAngle);
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
  horAngle  = 180 - (90 - station_azimuth + radToDeg(asin(dlon/dist_hor)));

#if MODE == DEBUG_ON
  Serial.print(F("vertAngle = "));
  Serial.println(vertAngle);
  Serial.print(F("horAngle = "));
  Serial.println(horAngle);
#endif
}

void writeServos(double vert, double hor){
  if (stateHor){
    servoHor.setTargetDeg(hor);
  }
  if(stateVert){
    servoVert.setTargetDeg(vert);
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
  station_azimuth = 0;
    
  if (debug) {
#if MODE == DEBUG_ON
    Serial.println("\n========COMPASS DATA========");
    Serial.print(F("station_azimuth = "));
    Serial.println(station_azimuth);
#endif
  }
}


#include "Adafruit_TCS34725.h"
#include "LineSensor.h"
#include "LightSensor.h"
#include <ESP32Servo.h>
#include "hcsr04.h"
#include "esp32-hal-ledc.h"
#include <ESP32Encoder.h> 

#define ROBOT_DEBUG_MODE 1
#define BUZ_PIN 23
#define BUZ_CHANNEL 0

#define LINE_PIN_0 39
#define LINE_PIN_1 34
#define LINE_PIN_2 35
#define LINE_PIN_3 32
#define LINE_PIN_4 33

#define BUTTON_PIN 19

#define LIGHT_SENSOR_PIN 36

Adafruit_TCS34725 * tcs;
uint8_t * usonic_trigger_pins;
uint8_t   usonic_count;
uint8_t * usonic_echo_pins;
uint8_t _esp_addr;

LineSensor * line_sensor;
LightSensor * light_sensor;
HCSR04* usonic_sensor[3];

unsigned long last_time;
unsigned long bzz_time;
unsigned long FREQ = 1200; 
unsigned long T = 1000000/FREQ;

bool start_sound = false;

int rq_cnt = 0;

const uint8_t R_PIN = 27;
const uint8_t G_PIN = 26;
const uint8_t B_PIN = 25;

void _ultrasonic_setup() {
    if (ROBOT_DEBUG_MODE) Serial.print("\nULTRASONIC SETUP... ");
    usonic_count        = 3;
    usonic_trigger_pins = new uint8_t[usonic_count] {13, 17, 18};
    usonic_echo_pins    = new uint8_t[usonic_count] {14, 15, 16};
    
    for (int i = 0; i < usonic_count; i++) 
        usonic_sensor[i] = new HCSR04(usonic_trigger_pins[i], usonic_echo_pins[i]);
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

/* SLAVE SENSOR */
void _button_setup() {
    if (ROBOT_DEBUG_MODE) Serial.print("BUTTON SETUP... ");
    pinMode(BUTTON_PIN, INPUT); 
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

bool button_is_pressed() {
    int button = digitalRead(BUTTON_PIN);
    return button == HIGH;
}

uint16_t inline make_uint16(uint8_t* buf) {
    return (buf[1] << 8) + buf[0];
}

void buzz(uint16_t frq) {
  ledcAttachPin(BUZ_PIN, BUZ_CHANNEL);
  ledcSetup( BUZ_CHANNEL, (uint32_t)frq, 8 );
  ledcWrite( BUZ_CHANNEL, 127 );
}

void noBuzz() {
  ledcDetachPin(BUZ_PIN);
  ledcWrite(BUZ_CHANNEL, 0);
}

inline bool check_bit(uint8_t byte, uint8_t bit, bool debug) {
    byte &= (1 << bit);
    if(bit > 7) {
        return false;
    }
    if (debug) {
        if (byte != 0 ) {
            if (ROBOT_DEBUG_MODE) Serial.printf ("1");
        } else {
            if (ROBOT_DEBUG_MODE) Serial.printf ("0");
        }
    }
    return byte;
}

void printLine(uint8_t b) {
  for (size_t i = 0; i < 5; i++) {
    check_bit(b, i, 1);
  }
  Serial.println("");
}

void _write_data_SENSOR() {
    uint8_t num_sensors = 3;
    if (ROBOT_DEBUG_MODE) Serial.printf("\nAnswer from ESP_SENSOR:\n");
    uint8_t send_buffer_local[64] = {0};

    /* -->[LINE]<--[2*USONIC][2*USONIC][2*USONIC][2*LIGHT][BUTTON] */
    uint8_t lin = line_sensor->get_data();
    Serial.printf("LINE_SENS: ");
    printLine(lin);
    uint16_t DISTANCE[num_sensors] = {0};

    /* [LINE]-->[2*USONIC][2*USONIC][2*USONIC]<--[2*LIGHT][BUTTON] */
   if (ROBOT_DEBUG_MODE) Serial.printf("\nUSONIC:\n");
    for (size_t i = 0; i < num_sensors; i++) {
        DISTANCE[i] = usonic_sensor[i]->distanceInMillimeters();
        if (ROBOT_DEBUG_MODE) Serial.printf("%d \t", DISTANCE[i]);
        /*
         * [LINE]-->[2*USONIC][2*USONIC][2*USONIC]<--[2*LIGHT][BUTTON]
         *   +1     +size(U16)+size(U16)+size(U16)
         */
    }
    uint16_t light_data = light_sensor->get_data();
    if (ROBOT_DEBUG_MODE) Serial.printf("LIGHT: %d \n", light_data);
    /*
     *   [LINE][2*USONIC][2*USONIC][2*USONIC]-->[2*LIGHT]<--[BUTTON]
     *     +1  +size(U16)+size(U16)+size(U16)
     */

    /*
     *   [LINE][2*USONIC][2*USONIC][2*USONIC][2*LIGHT ]-->[BUTTON]<--
     *     +1  +size(U16)+size(U16)+size(U16)+size(U16)
     */
   if (button_is_pressed()) {
      Serial.printf("BUTTON!\n");
    } else {
    } 
    
    if (ROBOT_DEBUG_MODE) Serial.printf("\n");
}


void setup() {
  
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);

  Serial.begin(115200);

  Serial.println("\nSLAVE SENSOR CONTROLLER");
  /* LINE SENSOR */
  line_sensor = new LineSensor(LINE_PIN_0, LINE_PIN_1, LINE_PIN_2, LINE_PIN_3, LINE_PIN_4); 
  /* ULTRASONIC SENSORS */
  _ultrasonic_setup();
  /* LIGHT SENSOR */
  light_sensor = new LightSensor(LIGHT_SENSOR_PIN); 
  /* BUTTON SETUP */
  _button_setup();
}

void loop() {
  _write_data_SENSOR();
  buzz(1000);
  analogWrite(R_PIN, 255);
  analogWrite(G_PIN, 255); 
  analogWrite(B_PIN, 255); 
  // Serial.printf("requests: %d\n", rq_cnt);
}

#include "Robot.h"
#include "LineSensor.h"
#include "LightSensor.h"
#include <ESP32Servo.h>
#include "hcsr04.h"
#include "esp32-hal-ledc.h"
#include <ESP32Encoder.h> 

#include "Arduino.h"

#define ROBOT_DEBUG_MODE 0

LineSensor * line_sensor;
LightSensor * light_sensor;
HCSR04* usonic_sensor[3];
ESP32Encoder * encoders;
Servo * servos;

volatile bool cur_button_state = false; 

const uint8_t SENSOR_PACK_SIZE = 11;
const uint8_t MOTOR_PACK_SIZE = 32;

const uint8_t ENCODER_CNT = 4;
const uint8_t SERVO_CNT = 9;

#define LINE_PIN_0 39
#define LINE_PIN_1 34
#define LINE_PIN_2 35
#define LINE_PIN_3 32
#define LINE_PIN_4 33

#define BUTTON_PIN 19

#define LIGHT_SENSOR_PIN 36
/*====================INTERNAL FUNCTIONS==========================*/
uint16_t inline make_uint16(uint8_t* buf) {
    return (buf[1] << 8) + buf[0];
}

int64_t inline make_int64(uint8_t* buf) {
    return (buf[7] << 56) + (buf[6] << 48) + (buf[5] << 40) + (buf[4] << 32) 
         + (buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + buf[0];
}

inline void uint16_to_bytes(uint16_t &val, uint8_t *bytes) {
  bytes[0] = val & 0xFF;                /* 0-7 bits */
  bytes[1] = (val & 0xFF00) >> 8;       /* 8-15 bits */
}

inline void int16_to_bytes(int16_t &val, uint8_t *bytes) {
  bytes[0] = val & 0xFF;                /* 0-7 bits */
  bytes[1] = (val & 0xFF00) >> 8;       /* 8-15 bits */
}

inline void int64_to_bytes(int64_t &val, uint8_t *bytes) {

  bytes[0] = val & 0xFF;                        /* 0-7 bits */
  bytes[1] = (val & 0xFF00) >> 8;               /* 8-15 bits */
  bytes[2] = (val & 0xFF0000) >> 16;            /* 16-23 bits */
  bytes[3] = (val & 0xFF000000) >> 24;          /* 24-31 bits */
  bytes[4] = (val & 0xFF00000000) >> 32;        /* 32-39 bits */
  bytes[5] = (val & 0xFF0000000000) >> 40;      /* 40-47 bits */
  bytes[6] = (val & 0xFF000000000000) >> 48;    /* 48-55 bits */
  bytes[7] = (val & 0xFF00000000000000) >> 56;  /* 56-63 bits */
}

bool button_is_pressed() {
    int button = digitalRead(BUTTON_PIN);
    return button == HIGH;
}

/* Возвращает значение бита, стоящего на данной позиции в данном байте */
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
/*================================================================*/


/*
 *  Конструктор инициализирует необходимые устройства в зависимости от того,
 *  какой контроллер прошивается.
 */
Robot::Robot(uint8_t type) {
    _esp_addr = type;
    memset(_receive_buffer_SENSOR, 0, sizeof(_receive_buffer_SENSOR));
    memset(_receive_buffer_MOTOR, 0, sizeof(_receive_buffer_MOTOR));
}

void Robot::init() {
    Serial.begin(115200);
    delay(50);
    Serial.printf("\n╔═══════════════╗\n");
    Serial.printf("║  CYBER RAVEN  ║\n");
    Serial.printf("╚═══════════════╝\n");
    Serial.println("\nStarting controller ...");
    last_command_time = millis(); /* Для проверки времмени между сообщениями */
    
    switch (_esp_addr)
    {
        case ESP_M:
        {
            if (ROBOT_DEBUG_MODE) Serial.println("MASTER CONTROLLER");

            tcs = new Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_1X);
            if (tcs->begin()) {
                Serial.println("Found color sensor!");
            } else {
                Serial.println("No TCS34725 found ... check your connections");
            }
            _master = true;
            _setup_master();
            break;
        }

        case ESP_SENSOR:
        {
            Serial.println("\nSLAVE SENSOR CONTROLLER");
            /* LINE SENSOR */
            line_sensor = new LineSensor(LINE_PIN_0, LINE_PIN_1, LINE_PIN_2, LINE_PIN_3, LINE_PIN_4); 
            /* ULTRASONIC SENSORS */
            _ultrasonic_setup();
            /* LIGHT SENSOR */
            light_sensor = new LightSensor(LIGHT_SENSOR_PIN); 
            /* BUTTON SETUP */
            _button_setup();
            _master = false;
            _esp_addr = ESP_SENSOR;
            _setup_slave();
            break;
        }
        
        case ESP_MOTOR:
        {
            Serial.println("\nSLAVE MOTOR CONTROLLER");
            _master = false;
            _esp_addr = ESP_MOTOR;
            _init_encoders();
            _init_motors();
            _setup_slave();
            break;
        }
        
        default:
        {
            while (1) {
                Serial.println("\nINVALID TYPE! Use Robot::ESP_M, ESP_SENSOR or ESP_MOTOR for definition");
                delay(500);
            }
            break;
        }
        
    }
}

/*
 *  SLAVE SENSOR
 *  [LINE][2*USONIC][2*USONIC][2*USONIC][2*LIGHT][BUTTON][CHSM]
 */
void Robot::_write_data_SENSOR() {
    uint8_t num_sensors = 3;
    if (ROBOT_DEBUG_MODE) Serial.printf("\nAnswer from ESP_SENSOR:");
    uint8_t send_buffer_local[64] = {0};

    /* -->[LINE]<--[2*USONIC][2*USONIC][2*USONIC][2*LIGHT][BUTTON] */
    send_buffer_local[0] = line_sensor->get_data();
    uint16_t DISTANCE[num_sensors] = {0};

    /* [LINE]-->[2*USONIC][2*USONIC][2*USONIC]<--[2*LIGHT][BUTTON] */
   if (ROBOT_DEBUG_MODE) Serial.printf("\nUSONIC:\n");
    for (size_t i = 0; i < num_sensors; i++) {
        DISTANCE[i] = usonic_sensor[i]->distanceInMillimeters();
        if (ROBOT_DEBUG_MODE) Serial.printf("DIST: %d \n", DISTANCE[i]);
        /*
         * [LINE]-->[2*USONIC][2*USONIC][2*USONIC]<--[2*LIGHT][BUTTON]
         *   +1     +size(U16)+size(U16)+size(U16)
         */
        uint16_to_bytes(DISTANCE[i], (send_buffer_local + 1) + i*sizeof(uint16_t));
    }
    uint16_t light_data = light_sensor->get_data();
    if (ROBOT_DEBUG_MODE) Serial.printf("LIGHT: %d \n", light_data);
    /*
     *   [LINE][2*USONIC][2*USONIC][2*USONIC]-->[2*LIGHT]<--[BUTTON]
     *     +1  +size(U16)+size(U16)+size(U16)
     */
    uint16_to_bytes(light_data, send_buffer_local + 1 + 3*sizeof(uint16_t)) ;

    /*
     *   [LINE][2*USONIC][2*USONIC][2*USONIC][2*LIGHT ]-->[BUTTON]<--
     *     +1  +size(U16)+size(U16)+size(U16)+size(U16)
     */
   if (button_is_pressed()) {
        send_buffer_local[SENSOR_PACK_SIZE-2] = 0xAA; /* TRUE */
        cur_button_state = true;
    } else {
        send_buffer_local[SENSOR_PACK_SIZE-2] = 0;
        cur_button_state = false;
    } 

    uint8_t checksum = 0;
    if (ROBOT_DEBUG_MODE) Serial.printf("\nCalculating checksum...\n");
    for (int i = 0; i < SENSOR_PACK_SIZE - 1 ; i++) {
        checksum += send_buffer_local[i];
        if (ROBOT_DEBUG_MODE) Serial.printf("{%u}", checksum);
    }
    if (ROBOT_DEBUG_MODE) Serial.printf("\n");
    send_buffer_local[SENSOR_PACK_SIZE-1] = checksum;

    for (int i = 0; i < SENSOR_PACK_SIZE ; i++) {
        Wire.write(send_buffer_local[i]);
        
    }
    if (ROBOT_DEBUG_MODE) {
        for (int i = 0; i < SENSOR_PACK_SIZE; i++) 
            if (ROBOT_DEBUG_MODE) Serial.printf("[%u]", send_buffer_local[i]);
    }
    if (ROBOT_DEBUG_MODE) Serial.printf("\n");
}

/* SLAVE MOTOR */
void Robot::_write_data_MOTOR() {
    Serial.printf("\nAnswer from ESP_MOTOR:");
    uint8_t buf_local[32] = {0};
    int64_t enc_data[ENCODER_CNT] = {0};

    for (size_t i = 0; i < ENCODER_CNT; i++) {
        enc_data[i] = encoders[i].getCount();
        int64_to_bytes(enc_data[i], buf_local + i*sizeof(int64_t));
    }

    for (int i = 0; i < MOTOR_PACK_SIZE; i++) {
        Wire.write(buf_local[i]);
        Serial.printf("[%u]", buf_local[i]);
    }
    
    Serial.println("");
}

Robot::~Robot(){}

/* SLAVE SENSOR */
void Robot::_ultrasonic_setup() {
    if (ROBOT_DEBUG_MODE) Serial.print("\nULTRASONIC SETUP... ");
    usonic_count        = 3;
    usonic_trigger_pins = new uint8_t[usonic_count] {13, 17, 18};
    usonic_echo_pins    = new uint8_t[usonic_count] {14, 15, 16};
    
    for (int i = 0; i < usonic_count; i++) 
        usonic_sensor[i] = new HCSR04(usonic_trigger_pins[i], usonic_echo_pins[i]);
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

/* SLAVE SENSOR */
void Robot::_button_setup() {
    if (ROBOT_DEBUG_MODE) Serial.print("BUTTON SETUP... ");
    pinMode(BUTTON_PIN, INPUT); 
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

/* SLAVE SENSOR/MOTOR */
void Robot::_setup_slave() {
    if (ROBOT_DEBUG_MODE) Serial.print("SETUP I2C... ");
    Wire.begin(_esp_addr);
    if (_esp_addr == ESP_SENSOR) Wire.onRequest(_write_data_SENSOR);
    if (_esp_addr == ESP_MOTOR) Wire.onRequest(_write_data_MOTOR);
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}
/* SLAVE MOTOR */
void Robot::_init_encoders() {
    delay(10);
    if (ROBOT_DEBUG_MODE) Serial.println("INIT ENCODERS");
    encoders = new ESP32Encoder[ENCODER_CNT];
    uint8_t out_enc_pin[8] = {4, 39, 17, 16, 34, 35, 36, 14};
    for (size_t i = 0; i < ENCODER_CNT; i++) {
        encoders[i].attachFullQuad(out_enc_pin[2*i], out_enc_pin[2*i+1]);
        encoders[i].setCount(0);
    }
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}

void Robot::_init_motors() {
    delay(10);
    if (ROBOT_DEBUG_MODE) Serial.println("INIT MOTORS");
    const int PWM_FREQ       = 40000;
    const int PWM_RESOLUTION = 8;
    for (int i = 0; i<=7; i++) {
        ledcSetup(i, PWM_FREQ, PWM_RESOLUTION);
    }

    ledcAttachPin((uint8_t)_motor::m1A, 0);
    ledcAttachPin((uint8_t)_motor::m1B, 1);
    ledcAttachPin((uint8_t)_motor::m2A, 2);
    ledcAttachPin((uint8_t)_motor::m2B, 3);
    ledcAttachPin((uint8_t)_motor::m3A, 4);
    ledcAttachPin((uint8_t)_motor::m3B, 5);
    ledcAttachPin((uint8_t)_motor::m4A, 6);
    ledcAttachPin((uint8_t)_motor::m4B, 7);
    if (ROBOT_DEBUG_MODE) Serial.println("COMPLETE!");
}
/*
 *   Метод, отвечающий за отправку заданных скоростей колес на контроллер MOTOR
 *   Вызывается мастер-контроллером в public-функциях для задания движения робота
 */
void Robot::_move(int16_t speed1, int16_t speed2, int16_t speed3, int16_t speed4){

    uint8_t spd_1[2] = {0};
    uint8_t spd_2[2] = {0};
    uint8_t spd_3[2] = {0};
    uint8_t spd_4[2] = {0};
    int16_to_bytes(speed1, spd_1);
    int16_to_bytes(speed2, spd_2);
    int16_to_bytes(speed3, spd_3);
    int16_to_bytes(speed4, spd_4);
    Wire.beginTransmission(ESP_MOTOR);

    Wire.write(spd_1[0]);
    Wire.write(spd_1[1]);

    Wire.write(spd_2[0]);
    Wire.write(spd_2[1]);

    Wire.write(spd_3[0]);
    Wire.write(spd_3[1]);

    Wire.write(spd_4[0]);
    Wire.write(spd_4[1]);

    Wire.endTransmission();
}

void Robot::_move(int16_t speed1, int16_t speed2, int16_t speed3, int16_t speed4, uint16_t timer_ms){

    uint8_t spd_1[2] = {0};
    uint8_t spd_2[2] = {0};
    uint8_t spd_3[2] = {0};
    uint8_t spd_4[2] = {0};
    uint8_t timer[2] = {0};
    int16_to_bytes(speed1, spd_1);
    int16_to_bytes(speed2, spd_2);
    int16_to_bytes(speed3, spd_3);
    int16_to_bytes(speed4, spd_4);
    uint16_to_bytes(timer_ms, timer);
    Wire.beginTransmission(ESP_MOTOR);

    Wire.write(spd_1[0]);
    Wire.write(spd_1[1]);

    Wire.write(spd_2[0]);
    Wire.write(spd_2[1]);

    Wire.write(spd_3[0]);
    Wire.write(spd_3[1]);

    Wire.write(spd_4[0]);
    Wire.write(spd_4[1]);

    Wire.write(timer[0]);
    Wire.write(timer[1]);

    Wire.endTransmission();
}

/* MASTER */
void Robot::_setup_master() {
    Serial.printf("Setup main ESP32..\n");
    servos = new Servo[9];
    uint8_t servo_pin[9] = {16, 17, 12, 13, 27, 26, 25, 33, 32};
    for (size_t i = 0; i < SERVO_CNT; i++) {
        servos[i].attach(servo_pin[i]);
    }

    RGB_state = new uint8_t[3] {0, 0, 0};
    isBuzzing = false;
    curFrq = 0;
    Wire.begin();
    Serial.printf("Complete!\n\n\n");
}

/* MASTER */
void Robot::_send_command(uint8_t dev_to, uint8_t data) {
    
    if(!_master) return;

    if (ROBOT_DEBUG_MODE) Serial.printf("\nPACKET TIME: %d\n", millis()-last_command_time);
    if ((millis() - last_command_time) < 20) return;
    last_command_time = millis();

    uint8_t recvd = Wire.requestFrom(dev_to, (size_t)data, false);
    if (ROBOT_DEBUG_MODE) Serial.printf("REQUEST TO: %u\n", dev_to);

    if (dev_to == ESP_SENSOR) {
        uint8_t bytes = 0;
        uint8_t temp_buffer[32] = {0};
        if (ROBOT_DEBUG_MODE) Serial.printf("GET DATA FROM DEVICE (10 - Sensors, 11 - Motors):\n");
        while (Wire.available()) {
            temp_buffer[bytes] = Wire.read();
            if (ROBOT_DEBUG_MODE) Serial.printf("[%u]", temp_buffer[bytes]);
            bytes++;
        }
        if (ROBOT_DEBUG_MODE) Serial.println("");
        _checksum(temp_buffer, SENSOR_PACK_SIZE, true);
    }
    
    if (dev_to == ESP_MOTOR)
    {
        uint8_t bytes = 0;
        while (Wire.available()) {
            _receive_buffer_MOTOR[bytes] = Wire.read();
            if (ROBOT_DEBUG_MODE) Serial.printf("[%u]", _receive_buffer_MOTOR[bytes]);
            bytes++;
        }
        if (ROBOT_DEBUG_MODE) Serial.println("");
    }
    
}

/*
 * Чексумма - последовательная сумма всех байт
 * Если пакет пришел без искажений, данные из пакета записываются 
 * в _receive_buffer_X, 
 * где X - контроллер, который определяется значением флага SENSOR
 */
void Robot::_checksum(uint8_t * temp_data, uint8_t size, bool SENSOR) {
    uint8_t chsm = 0;
    for (size_t i = 0; i < (size - 1); i++) {
        chsm += temp_data[i];
    } 
    if (ROBOT_DEBUG_MODE) Serial.printf("CHECKSUM: ");
    if (chsm == temp_data[size-1]) {
        if (ROBOT_DEBUG_MODE) Serial.printf("CORRECT!\n");
        SENSOR ? _data_to_buf(temp_data, true) : _data_to_buf(temp_data, false);
    } else {
        if (ROBOT_DEBUG_MODE) Serial.printf("INCORRECT!\n");
    }
}

void Robot::_data_to_buf(uint8_t * data, bool SENSOR) {
    if (SENSOR) {
        if (ROBOT_DEBUG_MODE) Serial.printf("DATA TO BUFFER SENSOR:\n");
        for (size_t i = 0; i < SENSOR_PACK_SIZE; i++) {
            _receive_buffer_SENSOR[i] = data[i];
            if (ROBOT_DEBUG_MODE) Serial.printf("[%u]", _receive_buffer_SENSOR[i]);
        }
    } else {
        if (ROBOT_DEBUG_MODE) Serial.printf("DATA TO BUFFER MOTOR:\n");
        for (size_t i = 0; i < MOTOR_PACK_SIZE; i++) {
            _receive_buffer_MOTOR[i] = data[i];
            if (ROBOT_DEBUG_MODE) Serial.printf("[%u]", _receive_buffer_MOTOR[i]);
        }
    }
}

/* Отправляем запрос на включение/выключение звука, состояние светодиода отправляем текущее */
void Robot::_sound_cmd(bool sound, uint16_t frequence) {
     if (ROBOT_DEBUG_MODE) Serial.printf("SND: %u ISBUZZ: %u\n",sound, isBuzzing);
    if (sound == isBuzzing && frequence == curFrq) return;

    uint8_t frq_bytes[2] = {0};
    uint16_to_bytes(frequence, frq_bytes);
    Wire.beginTransmission(ESP_SENSOR);  
    sound ? Wire.write(0xAA) : Wire.write(0);
    Wire.write(RGB_state[0]);
    Wire.write(RGB_state[1]);
    Wire.write(RGB_state[2]);
    Wire.write(frq_bytes[0]);
    Wire.write(frq_bytes[1]);
    Wire.endTransmission();
    isBuzzing = sound;
    curFrq = frequence;
}

/* Используются мастером для получения конкретных значений из байт в буфере */
uint16_t Robot::_parse_dev16(uint8_t device) {
    
    switch (device)
    {
        case (int)_device::ultrasonic_1:
        {
            return make_uint16(_receive_buffer_SENSOR + 1);
            break;
        }

        case (int)_device::ultrasonic_2:
        {
            return make_uint16(_receive_buffer_SENSOR + 1 + 1*sizeof(uint16_t));
            break;
        }

        case (int)_device::ultrasonic_3:
        {
            return make_uint16(_receive_buffer_SENSOR + 1 + 2*sizeof(uint16_t));
            break;
        }

        case (int)_device::light_sens:
        {
            return make_uint16(_receive_buffer_SENSOR + 1 + 3*sizeof(uint16_t));
            break;
        }
        
        default:
        {
            return -1;
            break;
        }
        
    }
    return -1;
}

uint8_t Robot::_parse_dev8(uint8_t device) {
    switch (device)
    {
        case (int)_device::line:
        {
            return _receive_buffer_SENSOR[0];
            break;
        }

        case (int)_device::button:
        {
            return _receive_buffer_SENSOR[SENSOR_PACK_SIZE - 2];
            break;
        }
    }
    return -1;
}

void Robot::_write_angle(Robot_servo & used_servo, int angle) {
    if (_check_angle(used_servo, angle)) {
        servos[used_servo.number].write(angle);
    } 
}

bool Robot::_check_angle(Robot_servo & used_servo, int angle) {
    if ((angle >= used_servo.low_bnd) && (angle <= used_servo.up_bnd)) return true;  
    Serial.printf(used_servo.name);
    Serial.printf(": Заданный угол %d° находится за допустимыми пределами [%u,%u] этого сервопривода!\n", angle, used_servo.low_bnd, used_servo.up_bnd);
    return false;
}

int64_t Robot::_enc_diff() {
    int64_t enc_difference[ENCODER_CNT] = {0};
    int64_t enc_cur[ENCODER_CNT] = {0} ;
    this->getEncoders(enc_cur);
    
    for (int i = 0; i < ENCODER_CNT; i++) {
        enc_difference[i] = enc_cur[i] - _enc_prev[i];
    }
    if (_enc_updated) {
        for (int i = 0; i < ENCODER_CNT; i++) {
            _enc_prev[i] = enc_cur[i];
        }
        _enc_updated = false;
    }
    return (enc_difference[0] + enc_difference[1] + enc_difference[2] + enc_difference[3])/ENCODER_CNT;
}

/*=========MASTER PUBLIC METHODS============*/
void Robot::update() {
    delay(20);
    _send_command(ESP_SENSOR, SENSOR_PACK_SIZE);
    delay(20);
    _send_command(ESP_MOTOR, MOTOR_PACK_SIZE);
    _dt = millis() - _last_enc_tp;
    _last_enc_tp = millis();
    _enc_updated = true;
}

void Robot::getLine(bool * array_for_line) {
    uint8_t b = _parse_dev8((uint8_t)_device::line);
    for (size_t i = 0; i < 5; i++) {
        array_for_line[i] = check_bit(b, i, 1);
    }
}

void Robot::getDistances(int * array_for_distances) {
    array_for_distances[0] = _parse_dev16((uint8_t)_device::ultrasonic_1);
    array_for_distances[1] = _parse_dev16((uint8_t)_device::ultrasonic_2);
    array_for_distances[2] = _parse_dev16((uint8_t)_device::ultrasonic_3);
}

int Robot::getLight() {
    return _parse_dev16((uint8_t)_device::light_sens);
}


bool Robot::getButton() {
    uint8_t bs = _parse_dev8((uint8_t)_device::button);
    if (bs == 0xAA) return true;
    if (bs == 0)    return false;
}

uint8_t Robot::getColor() {
    uint16_t c = 0;
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    tcs->getRawData(&r, &g, &b, &c);
    uint16_t sorted_colors[3] = {0};
    if (r > g && r > b) {
        sorted_colors[0] = r;
        sorted_colors[1] = max(g,b);
        sorted_colors[2] = min(g,b);
    }   
    if (g > r && g > b) {
        sorted_colors[0] = g;
        sorted_colors[1] = max(r,b);
        sorted_colors[2] = min(r,b);
    };   /* 1 - GREEN */
    if (b > g && b > r) {
        sorted_colors[0] = b;
        sorted_colors[1] = max(g,r);
        sorted_colors[2] = min(g,r);
    };   /* 2 - BLUE */
    
    /* Значение второй по интенсивности составляющей должно быть не более 85% от самой интенсивной составляющей */
    uint8_t max_value_precent = 85;
    if (sorted_colors[0] == r) {
        if ((uint32_t)sorted_colors[1] < max_value_precent*(uint32_t)sorted_colors[0]/100)
            return 0;
    } /* 0 - RED */ 

    if (sorted_colors[0] == g) {
        if ((uint32_t)sorted_colors[1] < max_value_precent*(uint32_t)sorted_colors[0]/100)
            return 1;
    } /* 1 - GREEN */

    if (sorted_colors[0] == b) {
        if ((uint32_t)sorted_colors[1] < max_value_precent*(uint32_t)sorted_colors[0]/100)
            return 2;

    } /* 2 - BLUE */
    if (sorted_colors[2] == b) {
        if ((uint32_t)sorted_colors[2] < max_value_precent*(uint32_t)sorted_colors[1]/100) {
            return 3;
        }
    } /* 3 - YELLOW */

    return -1;
}

void Robot::getColor(int &red, int &green, int &blue) {
    float f_r = 0.0;
    float f_g = 0.0;
    float f_b = 0.0;
    tcs->getRGB(&f_r, &f_g, &f_b);
    red = (int)f_r; 
    green = (int)f_g;
    blue = (int)f_b;
}

void Robot::SoundON(int frequence) {
    _sound_cmd(true, (uint16_t)frequence);
}
void Robot::SoundOFF() {
    _sound_cmd(false, 0);
}

/* Отправляем запрос на светодиод, состояние динамика отправляем текущее */
void Robot::RGB_cmd(uint8_t R, uint8_t G, uint8_t B) {
    if ((R == RGB_state[0]) &&(G == RGB_state[1]) && (B == RGB_state[2])) return;
    
    Wire.beginTransmission(ESP_SENSOR);  
    isBuzzing ? Wire.write(0xAA) : Wire.write(0);
    Wire.write(R);
    Wire.write(G);
    Wire.write(B);
    Wire.write(curFrq);
    Wire.endTransmission();
    RGB_state[0] = R;
    RGB_state[1] = G;
    RGB_state[2] = B;
}

void Robot::getEncoders(int64_t * array_for_encoders) {
    for (size_t i = 0; i < ENCODER_CNT; i++) {
        int64_t val = make_int64(_receive_buffer_MOTOR + i*8);
        array_for_encoders[i] = val;
    }
}

int32_t Robot::getOmniOdometry() {
    int64_t enc[ENCODER_CNT] = {0};

    double wheel_odometry_cm[4] = {0.0};
    double calc_odometry = 0.0;

    this->getEncoders(enc);

    for (int i = 0; i < ENCODER_CNT; i++) {
        wheel_odometry_cm[i] = (0.013780*(enc[i]) + 16.992811); /* Odometry equation for 60mm wheel */ 
        calc_odometry += wheel_odometry_cm[i];
    }
    int32_t output = (int32_t)calc_odometry/ENCODER_CNT;
    return output;
}

float Robot::getOdometryMeters() {
    int64_t enc[ENCODER_CNT] = {0};
    float spd = 0.035*(2*PI)*wheelRotatesPerSec();
    _distance += spd*((float)_dt/1000);
    return _distance;
}

float Robot::getOdometryMeters(uint32_t wheel_diameter_mm) {
    int64_t enc[ENCODER_CNT] = {0};
    float spd = ((float)wheel_diameter_mm/1000)*(2*PI)*wheelRotatesPerSec();
    _distance += spd*((float)_dt/1000);
    return _distance;
}

float Robot::wheelRotatesPerSec() {
    int64_t d_enc = _enc_diff();
    float enc_val_per_sec = (float)d_enc/(float)_dt;
    return (enc_val_per_sec*1000)/(_enc_in_360deg);
}

void Robot::moveCustom(int speedFR, int speedFL, int speedBL, int speedBR){
  Serial.println("Пользовательское движение");
  _move(speedFR, speedFL, speedBL, speedBR);
}

void Robot::moveCustom(int speedFR, int speedFL, int speedBL, int speedBR, uint16_t timer_msec){
  Serial.println("Пользовательское движение");
  _move(speedFR, speedFL, speedBL, speedBR, timer_msec);
}

void Robot::moveForward(int speed){
  Serial.println("Вперед");
  _move(speed, speed, speed, speed);
}

void Robot::moveBack(int speed){
  Serial.println("Назад");
  _move(-speed, -speed, -speed, -speed);
}

void Robot::moveRight(int speed){
  Serial.println("Движение вправо");
  _move(-speed, speed, -speed, speed);
}

void Robot::moveLeft(int speed){
  Serial.println("Движение влево");
  _move(speed, -speed, speed, -speed);
}

void Robot::rotLeft(int speed){
  Serial.println("Вращение против часовой стрелки");
  _move(speed, -speed, -speed, speed);
}

void Robot::rotRight(int speed){
  Serial.println("Вращение по часовой стрелке");
  _move(-speed, speed, speed, -speed);
}

void Robot::turnLeft(int speed, byte difference, bool forward) {
    Serial.println("Поворот налево");
    int decreased_spd = speed - ((speed * difference)/255);
    if (ROBOT_DEBUG_MODE) Serial.printf("\nMOVING LEFT: DIFF: %u, FWD: %u\n DECR_SPD: %d, D/255 = %f\n", difference, forward, decreased_spd, (float)difference/255);
    forward ? _move(speed, decreased_spd, decreased_spd, speed) : _move(-speed, -decreased_spd, -decreased_spd, -speed);
}

void Robot::turnRight(int speed, byte difference, bool forward) {
    Serial.println("Поворот направо");
    int decreased_spd = speed - ((speed * difference)/255);
    if (ROBOT_DEBUG_MODE) Serial.printf("\nMOVING RIGHT: DIFF: %u, FWD: %u\n DECR_SPD: %d, D/255 = %f\n", difference, forward, decreased_spd, (float)difference/255);
    forward ? _move(decreased_spd, speed, speed, decreased_spd) : _move(-decreased_spd, -speed, -speed, -decreased_spd) ;
}

void Robot::stopMoving(){
  Serial.println("Стоп");
  _move(0, 0, 0, 0);
}

void Robot::moveServo(use_servo my_servo, int angle) {
    switch (my_servo) {
        case use_servo::base : {
            _write_angle(servo_base, angle);
            break;
        }
            
        case use_servo::cam_left_right : {
            _write_angle(servo_cam_left_right, angle);
            break;
        }
            
        case use_servo::cam_up_down : {
            _write_angle(servo_cam_up_down, angle);
            break;
        }

        case use_servo::elbow : {
            _write_angle(servo_elbow, angle);
            break;
        }

        case use_servo::pitch : {
            _write_angle(servo_pitch, angle);
            break;
        }

        case use_servo::shoulder : {
            _write_angle(servo_shoulder, angle);
            break;
        }

        case use_servo::tool_back_manip : {
            _write_angle(servo_tool_back_manip, angle);
            break;
        }

        case use_servo::tool_front_manip : {
            _write_angle(servo_tool_front_manip, angle);
            break;
        }

        case use_servo::up_down_tool_front : {
            _write_angle(servo_up_down_tool_front, angle);
            break;
        }

        default:
            break;
    }
}

void Robot::moveToPose(byte base, byte shoulder, byte elbow, byte pitch) {
    if (!(_check_angle(servo_base, base) && _check_angle(servo_shoulder, shoulder) && _check_angle(servo_elbow, elbow) && _check_angle(servo_pitch, pitch))) {
        Serial.printf("Невозможно переместить манипулятор в указанную позицию: один из углов за допустимыми пределами!\n");
        delay(500);
        return;
    }
    _write_angle(servo_base, base);
    _write_angle(servo_shoulder, shoulder);
    _write_angle(servo_elbow, elbow);
    _write_angle(servo_pitch, pitch);
    
}
/*====================================*/

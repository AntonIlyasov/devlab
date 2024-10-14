#ifndef Robot_h
#define Robot_h
#include <stdint.h>

#include "Arduino.h"
#include "LineSensor.h"
#include "LightSensor.h"
#include "Adafruit_TCS34725.h"
#include "Wire.h"
#include "ESP32Servo.h"

/*
    Address of ESP SENSOR = 0xA (10) - SENSOR_ESP
    Address of ESP MOTOR = 0xB (11) - MOTOR_ESP

    Устройства, подключенные к ESP SENSOR:
        Датчик света :              1b
        Датчик линии :              1b
        Датчики расстояния (х3) :   3*2b
        Кнопка :                    0b
        Пьезодинамик :              0b
        Светодиод :                 0b

    Устройства, подключенные к ESP MOTOR:
        Моторы (х4) :               0b
        Энкодеры (х4) :             4*8b
    
    Датчик цвета (адрес - 0x29) :
*/


/* Основной класс для работы с роботом. */
class Robot
{
private:
    enum class _device {
        /* UINT8 */
         line            = 0x01,

        /* UINT16 */
         color           = 0x02,
         ultrasonic_1    = 0x13,
         ultrasonic_2    = 0x23,
         ultrasonic_3    = 0x33,
         light_sens      = 0x06,

         encoder         = 0x04,
         motor           = 0x05,
         sound           = 0x07,
         button          = 0x08
    } ;

    enum class _motor {
        
        m1A = 18,
        m1B = 23,
        m2A = 19,
        m2B = 13,
        m3A = 32,
        m3B = 33,
        m4A = 27,
        m4B = 26
    } ;

    typedef struct Robot_servo
    {
        uint8_t number;
        uint8_t low_bnd;
        uint8_t up_bnd;
        const char * name;
    };

    /* [LINE][2*USONIC][2*USONIC][2*USONIC][2*LIGHT] */ 
    static void _write_data_SENSOR();

    /* [ENC_1]*8[ENC_2]*8 */ 
    static void _write_data_MOTOR();

    uint8_t _parse_dev8(uint8_t device);
    uint16_t _parse_dev16(uint8_t device);
    int64_t _enc_diff();

    void _ultrasonic_setup();
    void _init_encoders();
    void _init_motors();
    void _move(int16_t speed1, int16_t speed2, int16_t speed3, int16_t speed4);
    void _move(int16_t speed1, int16_t speed2, int16_t speed3, int16_t speed4, uint16_t timer_ms);
    void _send_command(uint8_t esp_to, uint8_t data);
    void _setup_master();
    void _setup_slave();
    void _sound_cmd(bool sound, uint16_t frequence);
    void _button_setup();
    void _write_angle(Robot_servo & used_servo, int angle);
    bool _check_angle(Robot_servo & used_servo, int angle);
    
    void _checksum(uint8_t * temp_data, uint8_t size, bool A);
    void _data_to_buf(uint8_t * data, bool A);
    
    Adafruit_TCS34725 * tcs;
    uint8_t * usonic_trigger_pins;
    uint8_t   usonic_count;
    uint8_t * usonic_echo_pins;
    uint8_t _esp_addr;
    
    float _distance = 0.0;
    uint64_t last_command_time = 0;
    const uint32_t _enc_in_360deg = 1320;
    uint64_t _last_enc_tp = 0;
    uint64_t _dt;

    int64_t _enc_prev[4] = {0};
    
    bool _master;
    bool _enc_updated;
    uint8_t curFrq;
    uint8_t * RGB_state;
    bool isBuzzing;

    Robot_servo servo_base                {4, 0, 180, "Base"};
    Robot_servo servo_shoulder            {3, 0, 180, "Shoulder"};
    Robot_servo servo_elbow               {2, 0, 180, "Elbow"};
    Robot_servo servo_pitch               {1, 0, 180, "Pitch"};
    Robot_servo servo_cam_left_right      {5, 0, 180, "Cam left-right"};
    Robot_servo servo_cam_up_down         {6, 0, 180, "Cam up-down"};
    Robot_servo servo_tool_back_manip     {0, 90, 180, "Tool back"};
    Robot_servo servo_tool_front_manip    {7, 90, 180, "Tool front"};
    Robot_servo servo_up_down_tool_front  {8, 0, 180, "Tool front up-down"};
    
public:

    enum class use_servo {
        base,
        shoulder,
        elbow,
        pitch,
        cam_left_right,
        cam_up_down,
        tool_back_manip,
        tool_front_manip,
        up_down_tool_front,
    };
    /* Инициализирует робота, позволяя собирать данные с датчиков и отдавать команды */
    void init();

    /* Обновить состояния всех датчиков */
    void update();

    /* Состояние датчика линии. В функцию передается массив из 5 bool элементов. */
    void getLine(bool * array_for_line);
    /* Состояние датчиков расстояния. В функцию передается массив из 3 int элементов. */
    void getDistances(int * array_for_distances);

    /* Возвращает показатель освещенности */
    int getLight();

    /* Функция, сообщающая о том, нажата ли кнопка */
    bool getButton();

    /*
     *   Возвращает код цвета, который определил датчик цвета.
     *   0 - красный;
     *   1 - зеленый;
     *   2 - синий;
     *   3 - желтый;
     *   Если цвет определить не удалось, возвращает 255.
     */
    uint8_t getColor();

    /* 
     * Возвращает красную, зеленую и синюю составляющие цвета,
     * которые в дальнейшем можно обработать самостоятельно
     */
    void getColor(int &red, int &green, int &blue);

    /*Отправить команду на пьезодинамик. True - включить, False - выключить.*/
    void SoundON(int frequence);
    void SoundOFF();
    /*
     * Отправить команду на светодиод. 
     * Параметры R, G, B указывают, с какой интенсивностью должны светить 
     * красный, зеленый и синий каналы соответственно.
     * Диапазон для каждого канала: 0-255
     */
    void RGB_cmd(uint8_t R, uint8_t G, uint8_t B);

    void getEncoders(int64_t * array_for_encoders);

    int32_t getOmniOdometry();
    float wheelRotatesPerSec();
    float getOdometryMeters();
    float getOdometryMeters(uint32_t wheel_diameter_mm);

    
    void moveForward(int speed);
    void moveBack(int speed);
    void moveRight(int speed);
    void moveLeft(int speed);
    void rotLeft(int speed);
    void rotRight(int speed);
    /*
     * Поворот налево. Левые колеса будут вращаться медленнее правых на (difference/255)*speed.
     * При значении difference = 0 все колеса будут вращаться одинаково (движение по прямой).
     * При значении difference = 255 левые колеса не будут вращаться (поворот вокруг левых колес).
     */
    void turnLeft(int speed, byte difference, bool forward);

    /*
     *   Поворот направо. Правые колеса будут вращаться медленнее левых на (difference/255)*speed.
     *   При значении difference = 0 все колеса будут вращаться одинаково (движение по прямой).
     *   При значении difference = 255 правые колеса не будут вращаться (поворот вокруг правых колес).
     */
    void turnRight(int speed, byte difference, bool forward);
    void stopMoving();
    void moveCustom(int speedFR, int speedFL, int speedBL, int speedBR);
    void moveCustom(int speedFR, int speedFL, int speedBL, int speedBR, uint16_t timer_msec);

    /*
     *   Движение сервоприводов манипуляторов и камеры.
     *   сервоприводы base, shoulder, elbow, pitch, tool_back_manip - от основания до схвата заднего манипулятора.
     *   cam_left_right, cam_up_down - сервоприводы камеры.
     *   tool_front_manip - передний схват.
     *   up_down_tool_front - поднять/опустить передний схват.
     */
    void moveServo(use_servo my_servo, int angle);
    void moveToPose(byte base, byte shoulder, byte elbow, byte pitch);

    uint8_t _receive_buffer_SENSOR[64];
    uint8_t _receive_buffer_MOTOR[64];
    static const uint8_t ESP_SENSOR = 0xA;
    static const uint8_t ESP_MOTOR = 0xB;
    static const uint8_t ESP_M = 0xC;
    
/*
 *   Конструктор инициализирует необходимые устройства в зависимости от того,
 *   какой контроллер прошивается.
 */
    Robot(uint8_t type);
    ~Robot();
};

#endif


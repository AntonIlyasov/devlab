byte recvdArr[50];
int call_time = 0;

void updateSerial1()
{
  if (Serial1.available()) {
    for(int i = 0; i < 50; i++){
      recvdArr[i] = byte(Serial1.read());
      delay(10);
    }
  }
}

void setup()
{
  // start serial port
	Serial.begin(115200); 
	Serial.setTimeout(1); 
  while (!Serial) delay(10);

  Serial1.begin(115200);
  while (!Serial1) delay(10);

  // activate sensor
  Serial1.write(0xFF);
  delay(10);
  memset(recvdArr, 0, sizeof(recvdArr));
}

void loop()
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  if (Serial.available()>0){
    int command = Serial.readString().toInt(); 
    call_time = command * 1000;
    Serial.println("9");
  }

  updateSerial1();

  if (call_time){
    byte Hr = recvdArr[40];
    byte SPO = recvdArr[41];
    Serial.print(Hr);
    Serial.print(" ");
    Serial.println(SPO);
    memset(recvdArr, 0, sizeof(recvdArr));
    delay(call_time);
  }
}
#include <painlessMesh.h>

#define   MESH_PREFIX     "esp wifi"
#define   MESH_PASSWORD   "123456"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
bool enabled = 1;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

String nodeName = "stolb3";

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  int analogValue = analogRead(34);
  String msg = String(analogValue);
  size_t nodeId = 11111;
  mesh.sendSingle(nodeId, msg); 
}

void updateLed() ;

Task taskUpdateLed( TASK_SECOND * 0.1 , TASK_FOREVER, &updateLed );

void updateLed() {
  if (enabled) {
    int val = analogRead(34);
    val = map(val, 0, 4096, 0, 255);
    analogWrite(18, val);
    // analogWrite(19, val);
    // analogWrite(21, val);
  }
  else {
    analogWrite(18, 0);
    // analogWrite(19, 0);
    // analogWrite(21, 0);
  }
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
    if (msg == "0"){
    enabled = 0 ;
  }
  else{
    enabled = 1;
  }
  // Serial.println(enabled);
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);

  mesh.setDebugMsgTypes(ERROR | DEBUG);  // set before init() so that you can see error messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.setContainsRoot(true);
  mesh.onReceive(&receivedCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskUpdateLed );
  userScheduler.addTask( taskSendMessage );
  
  taskSendMessage.enable();
  taskUpdateLed.enable();
  Serial.printf("nodeId = ");
  Serial.println(mesh.getNodeId());

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  
}

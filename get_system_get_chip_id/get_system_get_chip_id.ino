#include <painlessMesh.h>

#define   MESH_SSID       "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;
uint32_t time_start = millis();

void setup() {
  Serial.begin(115200);
  mesh.init(MESH_SSID, MESH_PASSWORD, &userScheduler, MESH_PORT);
  
}

void loop() {
  Serial.println(mesh.getNodeId());
  while (millis() - time_start < 1000){;}
  time_start = millis();
}

#define RXD2 16
#define TXD2 17






void setup(void){
  Serial.begin(115200);
  while (!Serial) delay(10); 
  sim_card_setup();
  Serial.println("SUCCESS BOX SETUP");
}
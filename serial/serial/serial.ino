int x; 
void setup() { 
	Serial.begin(9600); 
	Serial.setTimeout(1); 
} 
void loop() { 
	// while (!Serial.available()); 

	if (Serial.available()>0){
		x = Serial.readString().toInt(); 
		Serial.println(x*2); 
	}


} 

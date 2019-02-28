int batTemp = 0 ;
int batStat =  0; 
void sendToSensorKit(); 

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
sendToSensorKit();
batStat++;
batTemp++;
delay(200);
}


void sendToSensorKit(){
String tempBat = String(batTemp);
String statBat = String(batStat);
String outData = "*"+statBat+"/"+tempBat+"?";
Serial.print(outData); 
}



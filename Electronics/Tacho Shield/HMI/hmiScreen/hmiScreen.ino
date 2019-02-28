#include "Nextion.h";
#define sensorSerial Serial1;

String inString = "";
String inRPM = "";
String inBatteryStat = "";
String inMotorTemp = "";

NexGauge Speedometer = NexGauge(0,5,"Speedometer");
NexGauge Tachometer = NexGauge(0,3,"Tachometer");
NexGauge BatteryStat = NexGauge(0,4,"BateryStat");
NexText BatteryStatText = NextText(0,8,"BatteryText");
NexText TachometerText = NextText(0,6,"TachometerText");
NexText SpeedText = NextText(0,7,"SpeedText");
NexText MotortempText = NextText(0,14,"MotortepText");
NexText BattempText = NextText(0,15,"BattempText");

void setup() {
  // put your setup code here, to run once:
nexInit();
sensorSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void sendRPM(){
  int rpm = getRPM();
  Tachometer.setValue(rpm);  
  }
int getRPM(){
  return ();
  }
  
void sendSpeed(){
  int speedValue = getSpeed();
  Speedometer.setValue(speedValue);
  }
int getSpeed(){
  return ();
  }
  
void sendCellTemp(){
  int cellTemp = getCellTemp();
  CellTemp.setValue(cellTemp);
}
int getCellTemp(){
  return ();
  }
  
void sendMosfetTemp(){
  int mTemp = getMosfetTemp();
  MosfetTemp.setValue(mTemp);
}
int getMosfetTemp(){
  return ();
  }

void readSensorSerial(){
  inString = sensorSerial.readStringUntil('/');
  }

#include <SoftwareSerial.h>
double numOfDetection = 0;
int timeLast = 0;
double rpm = 0;
char rpmText[5]; 
String rpmString;
int val = 0;
int time;

//SoftwareSerial ECU_Serial(0,1); // RX,TX
 

double numOfMagnets = 2;
void setup() {
pinMode(2,INPUT);
Serial.begin(9600);
attachInterrupt(0,updateDetection,RISING);
}

void loop() {
delay(100);  
detachInterrupt(0);
time = millis()-timeLast;
rpm = (numOfDetection/time)*60000;
timeLast = millis();
numOfDetection = 0;
convertRPMValue();
SerialDataWrite();


attachInterrupt(0,updateDetection,RISING);  

/*
val = digitalRead(2);
if(val==HIGH){
  digitalWrite(13,HIGH);
  numOfDetection++;
  digitalWrite(13,LOW);
  }

if(millis()-timeLast >= 1000){
  rpm = 60*numOfDetection/numOfMagnets;
  timeLast = millis();
  convertRPMValue();
  SerialDataWrite();
  numOfDetection = 0;
  delay(1000);
  digitalWrite(13,LOW);
  }
*/

}

void SerialDataWrite(){
  Serial.print(rpmString);
  }

void convertRPMValue(){
  rpmString = String(rpm,2);
  rpmString = rpmString+"/";
  }

void updateDetection(){
  numOfDetection++;
  }   

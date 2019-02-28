#include <PID_v1.h>
#include <SoftwareSerial.h>


SoftwareSerial motorDriverSerial_1(2,3); // RX, TX
SoftwareSerial motorDriverSerial_2(4,5); // RX, TX


#define tachometerSerial_2 Serial1
#define tachometerSerial_1 Serial
#define telemetriSerial Serial3
#define potentiometerPin 0
#define switchPin 2

int potValue = 0;
char potText[5] = {'0','0','0','0','/'} ;

int dir = 1;

int telemetriPot = 0;
int telemetriRPM = 0;

String rpmString_1 = "";
String rpmString_2 = "";
double actualRPM_1 = 0;
double actualRPM_2 = 0;
double desiredRPM_1 = 0;
double desiredRPM_2 = 0;

double PWM_1 = 0;
double PWM_2 = 0;

boolean stringComplete_1 = false; 
String inputString_1 = "";
boolean stringComplete_2 = false; 
String inputString_2 = "";


//PID motorDriverPID_1(&actualRPM_1,&PWM_1,&desiredRPM_1,1,2,5,DIRECT);
//PID motorDriverPID_2(&actualRPM_2,&PWM_2,&desiredRPM_2,1,2,5,DIRECT);

void setup() {

motorDriverSerial_1.begin(9600);
motorDriverSerial_2.begin(9600);

tachometerSerial_1.begin(9600);
tachometerSerial_2.begin(9600);

telemetriSerial.begin(9600);

pinMode(potentiometerPin,INPUT);   
pinMode(switchPin,INPUT);
   
inputString_1.reserve(10);
inputString_2.reserve(10);

//motorDriverPID_1.SetMode(AUTOMATIC);
//motorDriverPID_2.SetMode(AUTOMATIC);
}

void loop() {
readPotentiometer();
convertPotentiometer();
sendPWM();
readTachometer();
convertTachometer();
delay(100);
sendTelemetri();
}

void readPotentiometer(){
  potValue = analogRead(potentiometerPin);
  telemetriPot = potValue;
  potValue = potValue >> 2;
  if(digitalRead(switchPin) == LOW){
    potValue+=1000;
  }else
    potValue+=2000;
  }

void readTachometer(){
  if(stringComplete_1 == true){
    rpmString_1 = inputString_1;
    inputString_1 = "";
    stringComplete_1 = false;
    }
    
  if(stringComplete_2 == true){
    rpmString_2 = inputString_2;
    inputString_2 = "";
    stringComplete_2 = false;
    }  
  }

void convertTachometer(){
  actualRPM_1 = rpmString_1.toFloat();
  actualRPM_2 = rpmString_2.toFloat();
  }  
  
void sendTelemetri(){
    telemetriSerial.print(" RPM 1: ");
    telemetriSerial.print(actualRPM_2);
    telemetriSerial.print(" RPM 2: ");
    telemetriSerial.print(actualRPM_2);
  }
  
void convertPotentiometer(){
  potText[3] = potValue%10+'0';
  potValue/=10;
  potText[2] = potValue%10+'0';
  potValue/=10;
  potText[1] = potValue%10+'0';
  potValue/=10;
  potText[0] = potValue%10+'0';
  potText[4]='/';  
  }


void sendPWM(){
    for(int i = 0;i<5;i++){
      motorDriverSerial_1.write(potText[i]);
      motorDriverSerial_2.write(potText[i]);
      } 
  }

  void serialEvent1() {
  while (tachometerSerial_1.available()) {
  inputString_1 = tachometerSerial_1.readStringUntil('/');
  stringComplete_1 = true;
  }
  }
  void serialEvent2() {
  while (tachometerSerial_2.available()) {
  inputString_2 = tachometerSerial_2.readStringUntil('/');
  stringComplete_2 = true;
  }
  }

void calculateDesiredRPM(){
  desiredRPM_1= potValue;
  desiredRPM_2= potValue;
  }
  
void calculactePWM(){
  
  }

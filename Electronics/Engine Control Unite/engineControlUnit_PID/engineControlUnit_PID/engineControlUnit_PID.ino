#include <PID_v1.h>
#include <SoftwareSerial.h>

SoftwareSerial motorDriverSerial_1(2,3); // RX, TX
SoftwareSerial motorDriverSerial_2(4,5); // RX, TX

#define tachometerSerial_2 Serial2
#define tachometerSerial_1 Serial1

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

double rpm_1 = 10;
double rpm_2 = 10;

double PWM_1 = 0;
double PWM_2 = 0;

char pwmString_1[5] = {'0','0','0','0','/'} ;
char pwmString_2[5] = {'0','0','0','0','/'} ;

int pwmTemp_1 = 0;
int pwmTemp_2 = 0;

double desiredRPM_1;
double desiredRPM_2;

double actualRPM_1;
double actualRPM_2;

PID motorPID_1(&actualRPM_1,&PWM_1,&desiredRPM_1,1,2,5,DIRECT);
PID motorPID_2(&actualRPM_2,&PWM_2,&desiredRPM_2,1,2,5,DIRECT);
 
boolean stringComplete_1 = false; 
String inputString_1 = "";

boolean stringComplete_2 = false; 
String inputString_2 = "";

void setup() {

motorDriverSerial_1.begin(9600);
motorDriverSerial_2.begin(9600);

tachometerSerial_1.begin(9600);
tachometerSerial_2.begin(9600);

telemetriSerial.begin(9600);

pinMode(potentiometerPin,INPUT);   
pinMode(switchPin,INPUT);
pinMode(8,OUTPUT);
   
inputString_1.reserve(10);
inputString_2.reserve(10);

motorPID_1.SetMode(AUTOMATIC);
motorPID_2.SetMode(AUTOMATIC);
}

void loop() {
readPotentiometer();
readTachometer();
convertTachometer();
calculatePWM();
convertPWM();
sendTelemetri();
sendPWM();
delay(100);
}

void readPotentiometer(){
  potValue = analogRead(potentiometerPin);
  telemetriPot = potValue;
  potValue = potValue >> 2;
  desiredRPM_1 = potValue;
  desiredRPM_2 = potValue;
  
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
  actualRPM_1 = map(rpmString_1.toFloat(),0,5000,0,255);
  actualRPM_2= map(rpmString_2.toFloat(),0,5000,0,255);
  }  
  
void sendTelemetri(){
    telemetriSerial.print(" RPM 1: ");
    telemetriSerial.print(map(actualRPM_1,0,255,0,5000));
    telemetriSerial.print(" RPM 2: ");
    telemetriSerial.print(map(actualRPM_2,0,255,0,5000));

    
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
      motorDriverSerial_1.write(pwmString_1[i]);
      motorDriverSerial_2.write(pwmString_2[i]);
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
  
void calculatePWM(){
  motorPID_1.Compute();
  motorPID_2.Compute();
  }
  
void convertPWM(){
  pwmTemp_1 = (int)PWM_1;
  pwmTemp_2 = (int)PWM_2;

  if(digitalRead(switchPin) == LOW){
    pwmTemp_1+=1000;
    pwmTemp_2+=1000;
  }else{
    pwmTemp_1+=2000;
    pwmTemp_2+=2000;
  } 
  pwmString_1[3] = pwmTemp_1%10+'0';
  pwmTemp_1/=10;
  pwmString_1[2] = pwmTemp_1%10+'0';
  pwmTemp_1/=10;
  pwmString_1[1] = pwmTemp_1%10+'0';
  pwmTemp_1/=10;
  pwmString_1[0] = pwmTemp_1%10+'0';
  pwmString_1[4]='/';

  pwmString_2[3] = pwmTemp_2%10+'0';
  pwmTemp_2/=10;
  pwmString_2[2] = pwmTemp_2%10+'0';
  pwmTemp_2/=10;
  pwmString_2[1] = pwmTemp_2%10+'0';
  pwmTemp_2/=10;
  pwmString_2[0] = pwmTemp_2%10+'0';
  pwmString_2[4]='/';

  }

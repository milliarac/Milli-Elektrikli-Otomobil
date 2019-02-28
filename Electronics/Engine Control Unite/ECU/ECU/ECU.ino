#include <PID_v1.h>
#include <SoftwareSerial.h>

//Serial definitions
#define tachometerSerial1 Serial1
#define tachometerSerial2 Serial2
#define motorDriverSerial1 Serial3 
#define motorDriverSerial2 Serial
SoftwareSerial sensorKitSerial(52,53);

//Pin definitions
#define pedalPin1 0
#define pedalPin2 1
#define encoderPin 2
#define gearPin 51


//Constants
#define gasPedalTreshold 100
#define tresholdPID 50     
#define consKp 1
#define consKi 1
#define consKd 1
#define aggKp 5
#define aggKi 5
#define aggKd 5

//Varible definitions
int currMotor1 = 0;
int currMotor2 = 0;

int tempMotor1 = 0;
int tempMotor2 =  0;

int battStat = 0;

int speed = 0;

char gear = 'D';

int gasPedal = 0;
String gasPedalString = "0";

int rpm1 = 25;
int rpm2 = 125;
double rpmPID = 0;

double desRpm1 = 160;
double desRpm2 = 130;
double desRpmPID = 0;

double pwm1 = 0;
double pwm2 = 0;
double pwmPID = 0;
int angle = 0;

int counter = 0;

//Variables for eds
const double diameter = 20; 
const double dw = 15;
const double lw = 15;
const int maxRPM = 1000;

//PID cons
PID motorPID1(&rpmPID, &pwmPID, &desRpmPID, consKp, consKi, consKd, DIRECT);
PID motorPID2(&rpmPID, &pwmPID, &desRpmPID, consKp, consKi, consKd, DIRECT);

void setup() {
sensorKitSerial.begin(115200);
tachometerSerial1.begin(115200);
tachometerSerial2.begin(115200);
motorDriverSerial1.begin(115200);
motorDriverSerial2.begin(115200);
motorPID1.SetMode(AUTOMATIC);
motorPID2.SetMode(AUTOMATIC);
pinMode(gearPin,INPUT);
}

void loop() {
//readGear();
//readGasPedal();
//readTachometer1();
//readTachometer2();
//readEncoder();
//calculateSpeed();
//calculateDesiredRPM();
computePID1();
computePID2();
sendToSensorKit();
sendPWM();
delay(200);
}

void readGasPedal(){
int gas1 = analogRead(pedalPin1);
int gas2 = analogRead(pedalPin2);

if(abs(gas1-gas2) > gasPedalTreshold){
  gasPedal = min(gas1,gas2);
  }else{
    gasPedal= gas1;
    }
gasPedal = map(gasPedal,0,1024,0,255);
}

void readTachometer1(){
  if(tachometerSerial1.available()){
    String inData1 = tachometerSerial1.readStringUntil('?');
	char inDataArray[inData.length()+1];
    inData.toCharArray(inDataArray,inData.length()+1);
    sscanf(inDataArray, "*%d/%d",&rpm1,&tempMotor1);
	}   
  }

  /*
void readTachometer2(){
    if(tachometerSerial2.available()){
    String inData2 = tachometerSerial2.readStringUntil('?');
    rpm2 = inData2.substring(1).toInt();
    }
  }
*/

void readMotorCurrent1(){
  if(motorDriverSerial1.available()){
    String inData1 = motorDriverSerial1.readStringUntil('?');
    currMotor1 = inData1.substring(1).toInt();
    Serial.println(currMotor1);
    }    
  }

void readMotorCurrent2(){
  if(motorDriverSerial2.available()){
    String inData2 = motorDriverSerial1.readStringUntil('?');
    currMotor2 = inData2.substring(1).toInt();
    }    
  }
  
void sendPWM(){
  if(motorDriverSerial1.available()){
    String strPWM1 = String((int)pwm1);
    motorDriverSerial1.print("*"+strPWM1+"?");
    }
  if(motorDriverSerial2.available()){
    String strPWM2 = String((int)pwm2);
    motorDriverSerial1.print("*"+strPWM2+"?");
    }  
  }

void calculateSpeed(){
  speed = (int)(rpm1+rpm2)/2*diameter;
  }
void sendToSensorKit(){
  String currMotorString1 = String(currMotor1);
  String currMotorString2 = String(currMotor2);
  String speedString = String((int)speed);
  String tempMotorString1 = String(tempMotor1);
  String tempMotorString2 = String(tempMotor2);
  String outData = "*"+currMotorString1+"/"+currMotorString2+"/"+speedString+"/"+tempMotorString1+"/"+tempMotorString2+"/"+gear+"?";
  sensorKitSerial.print(outData);
}

void readGear(){
  if(digitalRead(gearPin) == LOW)
    gear = 'R';
  else
    gear = 'D';
  }

void readEncoder(){
  angle = analogRead(encoderPin);
  angle = map(angle , 0, 1024, 0, 360);
  } 

void calculateDesiredRPM(){
  //calculations according to formula
  double gasPedal_rad;
  double deltaW = (dw*tan(angle)/lw)*gasPedal_rad;
  if(angle > 180){ 
  desRpm1 = gasPedal_rad - deltaW; 
  desRpm2 = gasPedal_rad;
  }else{
  desRpm2 = gasPedal_rad - deltaW; 
  desRpm1 = gasPedal_rad;

  //Mapping for PID calculations
  desRpm1 = map(desRpm1,0,maxRPM,0,255);
  desRpm2 = map(desRpm2,0,maxRPM,0,255);
  
    }
  }

void computePID1(){
  
  double gap1 = abs(rpm1-desRpm1);
  desRpmPID = desRpm1;
  rpmPID = rpm1;
  pwmPID = pwm1;
  
  if(gap1 > tresholdPID)
    motorPID1.SetTunings(aggKp,aggKi,aggKd);
  else
    motorPID1.SetTunings(consKp,consKi,consKd);
    
  Serial.println("_____________1________________");
  Serial.println(rpmPID);
  Serial.println(desRpmPID);
  motorPID1.Compute();
  pwm1 = pwmPID;
  
  Serial.print("PWM1 :");
  Serial.println(pwm1);

  }

void computePID2(){
  
  double gap2 = abs(rpm2-desRpm2);
  desRpmPID = desRpm2;
  rpmPID = rpm2;
  pwmPID = pwm2;
  if(gap2 > tresholdPID)
    motorPID2.SetTunings(aggKp,aggKi,aggKd);
  else
    motorPID2.SetTunings(consKp,consKi,consKd);

  Serial.println("________________--2_____________");
  Serial.println(rpmPID);
  Serial.println(desRpmPID);
  motorPID2.Compute();
  pwm2 = pwmPID;
  
  Serial.print("PWM2 :");
  Serial.println(pwm2);
  }
  

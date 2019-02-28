#include <SPI.h>

const int slaveA = 1;
const int slaveB = 2;
const int slaveC = 3;
const int slaveD = 4;

uint8_t stat, val1, val2, valueA, valueB, valueC, valueD;

SPISettings settingsA(2000000, MSBFIRST, SPI_MODE1);
SPISettings settingsB(2000000, MSBFIRST, SPI_MODE1);
SPISettings settingsC(2000000, MSBFIRST, SPI_MODE1);
SPISettings settingsD(2000000, MSBFIRST, SPI_MODE1);

void setup() {
  // put your setup code here, to run once:
pinMode(slaveA,OUTPUT);
pinMode(slaveB,OUTPUT);
pinMode(slaveC,OUTPUT);
pinMode(slaveD,OUTPUT);  
SPI.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
for(int i = 1;i < 5 ; i++){
  listenSlaves(i);
  Serial.println(valueA);
  Serial.println(valueB);
  Serial.println(valueC);
  Serial.println(valueD);
  }
}

void listenSlaves(int slaveNo){
  switch(slaveNo){

  case(1):
  SPI.beginTransaction(settingsA);
  digitalWrite(slaveA,LOW);
  stat = SPI.transfer(0);
  val1 = SPI.transfer(0);
  val2 = SPI.transfer(0);
  digitalWrite(slaveB,HIGH);
  SPI.endTransaction();
  if(stat == 1)
    valueA = val1;
  else if(stat == 2)
    valueA = val2;
  else
    valueA = 0;
  
  case(2):
  SPI.beginTransaction(settingsB);
  digitalWrite(slaveB,LOW);
  stat = SPI.transfer(0);
  val1 = SPI.transfer(0);
  val2 = SPI.transfer(0);
  digitalWrite(slaveB,HIGH);
  SPI.endTransaction();
  if(stat == 1)
    valueB = val1;
  else if(stat == 2)
    valueB = val2;
  else
    valueB = 0;      

  case(3):
  SPI.beginTransaction(settingsC);
  digitalWrite(slaveC,LOW);
  stat = SPI.transfer(0);
  val1 = SPI.transfer(0);
  val2 = SPI.transfer(0);
  digitalWrite(slaveC,HIGH);
  SPI.endTransaction();
  if(stat == 1)
    valueC = val1;
  else if(stat == 2)
    valueC = val2;
  else
    valueC = 0;

  case(4):
  SPI.beginTransaction(settingsD);
  digitalWrite(slaveD,LOW);
  stat = SPI.transfer(0);
  val1 = SPI.transfer(0);
  val2 = SPI.transfer(0);
  digitalWrite(slaveD,HIGH);
  SPI.endTransaction();
  if(stat == 1)
    valueD = val1;
  else if(stat == 2)
    valueD = val2;
  else
    valueD = 0;

  }    
}

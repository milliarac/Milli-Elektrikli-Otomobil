#include <SPI.h>

#define MISO 12
#define MOSI 11 
#define CS 10 
#define CLK 13

#define WRCFG 0x01 //Write Configuration Registers
#define RDCFG 0x02 // Read config
#define RDCV 0x04 // Read cells
#define STCVAD 0x10 // Start all A/D's - poll status
#define RDFLG 0x06 //Read Flags
#define RDTMP 0x08 //Read Temperatures
#define STCDC 0x60 //A/D converter and poll Status
#define STOWAD 0x20 //Start Test - poll status
#define STTMPAD 0x30// Temperature Reading - ALL

#define MAX_CELLS  12

byte address[3] = {0,1,2};

byte CFGR0,CFGR1,CFGR2,CFGR3,CFGR4,CFGR5;
double voltage[3][12] = {(0,0,0,0,0,0,0,0,0,0,0,0),(0,0,0,0,0,0,0,0,0,0,0,0),(0,0,0,0,0,0,0,0,0,0,0,0)};
double lowValue[3];
double lowIndex[3];


void setup() {
  // put your setup code here, to run once:


}

void loop() {
  // put your main code here, to run repeatedly:
readVoltage(0);
readVoltage(1);
readVoltage(2);

findLowestCell(0);
findLowestCell(1);
findLowestCell(2);

dischargeCells(0);
dischargeCells(1);
dischargeCells(2);
}

void sendToSensorKit(){
  
  }

void setupSPI(){
  pinMode(MISO,INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(CLK,OUTPUT);
  digitalWrite(CS,HIGH);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.begin();
  }

void readVoltage(int i){
  pinMode(CS,LOW);
  SPI.transfer(STCVAD);
  delay(20);
  pinMode(CS,HIGH);
  
  byte volt[18];
  
  pinMode(CS,LOW);
  SPI.transfer(address[i]);
  SPI.transfer(RDCV);
  for(int j = 0;j < 18; j++){
    volt[j] = SPI.transfer(RDCV);
    }
  pinMode(CS,HIGH);

  //Burası düzeltilcek................................................................................
  voltage[i][0] = (((volt[0] & 0xFF) | (volt[1] & 0x0F) << 8)*1.5*0.001);
  voltage[i][1] = (((volt[1] & 0xF0) >> 4 | (volt[2] & 0xFF) << 4)*1.5*0.001); 
  voltage[i][2] = (((volt[3] & 0xFF) | (volt[4] & 0x0F) << 8)*1.5*0.001);
  voltage[i][3] = (((volt[4] & 0xF0) >> 4 | (volt[5] & 0xFF) << 4)*1.5*0.001); 
  voltage[i][4] = (((volt[6] & 0xFF) | (volt[5] & 0x0F) << 8)*1.5*0.001);
  voltage[i][5] = (((volt[8] & 0xF0) >> 4 | (volt[5] & 0xFF) << 4)*1.5*0.001);
  voltage[i][6] = (((volt[10] & 0xFF) | (volt[1] & 0x0F) << 8)*1.5*0.001);
  voltage[i][7] = (((volt[12] & 0xF0) >> 4 | (volt[2] & 0xFF) << 4)*1.5*0.001); 
  voltage[i][8] = (((volt[14] & 0xFF) | (volt[2] & 0x0F) << 8)*1.5*0.001);
  voltage[i][9] = (((volt[16] & 0xF0) >> 4 | (volt[3] & 0xFF) << 4)*1.5*0.001); 
  voltage[i][10] = (((volt[16] & 0xFF) | (volt[4] & 0x0F) << 8)*1.5*0.001);
  voltage[i][11] = (((volt[1] & 0xF0) >> 4 | (volt[5] & 0xFF) << 4)*1.5*0.001); 
  }
  
void findLowestCell(int i){
  int index = 0;
  double lowestVoltage = 5;
  for(int j= 0; j<MAX_CELLS; j++){
    if(voltage[i][j] < lowestVoltage){
      lowestVoltage = voltage[i][j];
      index = i;
      }
    }
  lowIndex[i] = index;
  lowValue[i] = lowestVoltage;
  }

void dischargeCells(int i){
  CFGR1 = 0x00;
  CFGR2 = 0x00;
  double lowest = min(min(lowValue[0],lowValue[1]),lowValue[2]);
  for(int j = 0; j< MAX_CELLS; j++){
    double diff = voltage[i][j] - lowest;
    if(j <= 8 && diff > 0.03)   //discharge if difference more than 0.03V
    {
    CFGR1 += 0x01<<(j);
    }
    if(diff != 0 && j <= 8 && diff <= 0.03)
    {
    CFGR1 += 0x00<<(j);  
    }
    if(j > 8 && diff > 0.03)
    {
    CFGR2 += 0x01<<(j-8);
    }
    if(diff != 0 && j > 8 && diff <= 0.03)
    {
    CFGR2 += 0x00<<(j-8);
    }
    }
    writeConfRegister(i);
  }

  void writeConfRegister(int i){
    digitalWrite(CS,LOW);
    
    SPI.transfer(address[i]);
    SPI.transfer(WRCFG);
    SPI.transfer(CFGR0);
    SPI.transfer(CFGR1);
    SPI.transfer(CFGR2);
    SPI.transfer(CFGR3);
    SPI.transfer(CFGR4);
    SPI.transfer(CFGR5);
    
    digitalWrite(CS,HIGH);
    }

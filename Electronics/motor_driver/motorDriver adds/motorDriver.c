unsigned short hall;
unsigned char MoveTableF[8] = {0, 33, 6, 36, 24, 9, 18, 0};
unsigned char MoveTableR[8] = {0, 18, 9, 24, 36, 6, 33, 0};
char receivedText[5]={'1','0','0','0','/'};
int dir=1;
char amp[3]={0,0,0};
int amp_int=0;



//PWM ratio update function
void updatePWM(){
PWM1_Set_Duty(amp_int);
amp_int=0;
PWM1_Start();
}
//Hall sensor reading function
void readHallSensors(){
hall = PORTB;
hall = hall & 112;
hall = hall >> 4;
}

//MotorPins update function
void updateMotorPins(){
if(dir==1)
PORTD = MoveTableF[hall];
else if(dir==2)
PORTD = MoveTableR[hall];
}
//Uart data receiving function
void serialDataRead(){
int i=0;
while(i<5){
 if(UART1_Data_Ready())
 {
  receivedText[i]=UART1_Read();
  i++;
 }
}

}
//Data converting function
void convertReceivedData()
{
if(receivedText[4]=='/'){
dir=receivedText[0]-'0';
amp_int+=(receivedText[1]-'0')*100;
amp_int+=(receivedText[2]-'0')*10;
amp_int+=receivedText[3]-'0';
}
else
SerialDataRead();

}
//Stop Button Function
void Interrupt(){
  if(INTF_bit){
  INTCON = 0;
  PORTB.F2 = 0;
  PORTD = 0;
  PWM1_Stop();
  }

  if (RBIF_bit){
  RBIF_bit = 0;
  readHallSensors();
  updateMotorPins();
  }
}
void InitInterrupts(){
  OPTION_REG = 0;
  TMR1IE_bit    = 0;
  INTCON        = 0xD8;
}

void main() {
 ADCON1 = 14;
 CMCON  = 7;
 PORTB = 0;
 TRISB = 243;
 PORTD = 0;
 TRISD = 0;
 InitInterrupts();
 PWM1_Init(300);
 UART1_Init(9600);


 while(1){
  PORTB.F2 = 1;
  serialDataRead();
  convertReceivedData();
  PORTB.F2 = 0;
  readHallSensors();
  updateMotorPins();
  PORTB.F2 = 0;
  updatePWM();
 }
}

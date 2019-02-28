
_updatePWM:

;motorDriver.c,11 :: 		void updatePWM(){
;motorDriver.c,12 :: 		PWM1_Set_Duty(amp_int);
	MOVF       _amp_int+0, 0
	MOVWF      FARG_PWM1_Set_Duty_new_duty+0
	CALL       _PWM1_Set_Duty+0
;motorDriver.c,13 :: 		amp_int=0;
	CLRF       _amp_int+0
	CLRF       _amp_int+1
;motorDriver.c,14 :: 		PWM1_Start();
	CALL       _PWM1_Start+0
;motorDriver.c,15 :: 		}
L_end_updatePWM:
	RETURN
; end of _updatePWM

_readHallSensors:

;motorDriver.c,17 :: 		void readHallSensors(){
;motorDriver.c,18 :: 		hall = PORTB;
	MOVF       PORTB+0, 0
	MOVWF      _hall+0
;motorDriver.c,19 :: 		hall = hall & 112;
	MOVLW      112
	ANDWF      _hall+0, 1
;motorDriver.c,20 :: 		hall = hall >> 4;
	RRF        _hall+0, 1
	BCF        _hall+0, 7
	RRF        _hall+0, 1
	BCF        _hall+0, 7
	RRF        _hall+0, 1
	BCF        _hall+0, 7
	RRF        _hall+0, 1
	BCF        _hall+0, 7
;motorDriver.c,21 :: 		}
L_end_readHallSensors:
	RETURN
; end of _readHallSensors

_updateMotorPins:

;motorDriver.c,24 :: 		void updateMotorPins(){
;motorDriver.c,25 :: 		if(dir==1)
	MOVLW      0
	XORWF      _dir+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__updateMotorPins15
	MOVLW      1
	XORWF      _dir+0, 0
L__updateMotorPins15:
	BTFSS      STATUS+0, 2
	GOTO       L_updateMotorPins0
;motorDriver.c,26 :: 		PORTD = MoveTableF[hall];
	MOVF       _hall+0, 0
	ADDLW      _MoveTableF+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
	GOTO       L_updateMotorPins1
L_updateMotorPins0:
;motorDriver.c,27 :: 		else if(dir==2)
	MOVLW      0
	XORWF      _dir+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__updateMotorPins16
	MOVLW      2
	XORWF      _dir+0, 0
L__updateMotorPins16:
	BTFSS      STATUS+0, 2
	GOTO       L_updateMotorPins2
;motorDriver.c,28 :: 		PORTD = MoveTableR[hall];
	MOVF       _hall+0, 0
	ADDLW      _MoveTableR+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTD+0
L_updateMotorPins2:
L_updateMotorPins1:
;motorDriver.c,29 :: 		}
L_end_updateMotorPins:
	RETURN
; end of _updateMotorPins

_serialDataRead:

;motorDriver.c,31 :: 		void serialDataRead(){
;motorDriver.c,32 :: 		int i=0;
	CLRF       serialDataRead_i_L0+0
	CLRF       serialDataRead_i_L0+1
;motorDriver.c,33 :: 		while(i<5){
L_serialDataRead3:
	MOVLW      128
	XORWF      serialDataRead_i_L0+1, 0
	MOVWF      R0+0
	MOVLW      128
	SUBWF      R0+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__serialDataRead18
	MOVLW      5
	SUBWF      serialDataRead_i_L0+0, 0
L__serialDataRead18:
	BTFSC      STATUS+0, 0
	GOTO       L_serialDataRead4
;motorDriver.c,34 :: 		if(UART1_Data_Ready())
	CALL       _UART1_Data_Ready+0
	MOVF       R0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_serialDataRead5
;motorDriver.c,36 :: 		receivedText[i]=UART1_Read();
	MOVF       serialDataRead_i_L0+0, 0
	ADDLW      _receivedText+0
	MOVWF      FLOC__serialDataRead+0
	CALL       _UART1_Read+0
	MOVF       FLOC__serialDataRead+0, 0
	MOVWF      FSR
	MOVF       R0+0, 0
	MOVWF      INDF+0
;motorDriver.c,37 :: 		i++;
	INCF       serialDataRead_i_L0+0, 1
	BTFSC      STATUS+0, 2
	INCF       serialDataRead_i_L0+1, 1
;motorDriver.c,38 :: 		}
L_serialDataRead5:
;motorDriver.c,39 :: 		}
	GOTO       L_serialDataRead3
L_serialDataRead4:
;motorDriver.c,41 :: 		}
L_end_serialDataRead:
	RETURN
; end of _serialDataRead

_convertReceivedData:

;motorDriver.c,43 :: 		void convertReceivedData()
;motorDriver.c,45 :: 		if(receivedText[4]=='/'){
	MOVF       _receivedText+4, 0
	XORLW      47
	BTFSS      STATUS+0, 2
	GOTO       L_convertReceivedData6
;motorDriver.c,46 :: 		dir=receivedText[0]-'0';
	MOVLW      48
	SUBWF      _receivedText+0, 0
	MOVWF      _dir+0
	CLRF       _dir+1
	BTFSS      STATUS+0, 0
	DECF       _dir+1, 1
;motorDriver.c,47 :: 		amp_int+=(receivedText[1]-'0')*100;
	MOVLW      48
	SUBWF      _receivedText+1, 0
	MOVWF      R0+0
	CLRF       R0+1
	BTFSS      STATUS+0, 0
	DECF       R0+1, 1
	MOVLW      100
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	CALL       _Mul_16X16_U+0
	MOVF       R0+0, 0
	ADDWF      _amp_int+0, 1
	MOVF       R0+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      _amp_int+1, 1
;motorDriver.c,48 :: 		amp_int+=(receivedText[2]-'0')*10;
	MOVLW      48
	SUBWF      _receivedText+2, 0
	MOVWF      R0+0
	CLRF       R0+1
	BTFSS      STATUS+0, 0
	DECF       R0+1, 1
	MOVLW      10
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	CALL       _Mul_16X16_U+0
	MOVF       R0+0, 0
	ADDWF      _amp_int+0, 1
	MOVF       R0+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      _amp_int+1, 1
;motorDriver.c,49 :: 		amp_int+=receivedText[3]-'0';
	MOVLW      48
	SUBWF      _receivedText+3, 0
	MOVWF      R0+0
	CLRF       R0+1
	BTFSS      STATUS+0, 0
	DECF       R0+1, 1
	MOVF       R0+0, 0
	ADDWF      _amp_int+0, 1
	MOVF       R0+1, 0
	BTFSC      STATUS+0, 0
	ADDLW      1
	ADDWF      _amp_int+1, 1
;motorDriver.c,50 :: 		}
	GOTO       L_convertReceivedData7
L_convertReceivedData6:
;motorDriver.c,52 :: 		SerialDataRead();
	CALL       _serialDataRead+0
L_convertReceivedData7:
;motorDriver.c,54 :: 		}
L_end_convertReceivedData:
	RETURN
; end of _convertReceivedData

_Interrupt:
	MOVWF      R15+0
	SWAPF      STATUS+0, 0
	CLRF       STATUS+0
	MOVWF      ___saveSTATUS+0
	MOVF       PCLATH+0, 0
	MOVWF      ___savePCLATH+0
	CLRF       PCLATH+0

;motorDriver.c,56 :: 		void Interrupt(){
;motorDriver.c,57 :: 		if(INTF_bit){
	BTFSS      INTF_bit+0, BitPos(INTF_bit+0)
	GOTO       L_Interrupt8
;motorDriver.c,58 :: 		INTCON = 0;
	CLRF       INTCON+0
;motorDriver.c,59 :: 		PORTB.F2 = 0;
	BCF        PORTB+0, 2
;motorDriver.c,60 :: 		PORTD = 0;
	CLRF       PORTD+0
;motorDriver.c,61 :: 		PWM1_Stop();
	CALL       _PWM1_Stop+0
;motorDriver.c,62 :: 		}
L_Interrupt8:
;motorDriver.c,64 :: 		if (RBIF_bit){
	BTFSS      RBIF_bit+0, BitPos(RBIF_bit+0)
	GOTO       L_Interrupt9
;motorDriver.c,65 :: 		RBIF_bit = 0;
	BCF        RBIF_bit+0, BitPos(RBIF_bit+0)
;motorDriver.c,66 :: 		readHallSensors();
	CALL       _readHallSensors+0
;motorDriver.c,67 :: 		updateMotorPins();
	CALL       _updateMotorPins+0
;motorDriver.c,68 :: 		}
L_Interrupt9:
;motorDriver.c,69 :: 		}
L_end_Interrupt:
L__Interrupt21:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _Interrupt

_InitInterrupts:

;motorDriver.c,70 :: 		void InitInterrupts(){
;motorDriver.c,71 :: 		OPTION_REG = 0;
	CLRF       OPTION_REG+0
;motorDriver.c,72 :: 		TMR1IE_bit    = 0;
	BCF        TMR1IE_bit+0, BitPos(TMR1IE_bit+0)
;motorDriver.c,73 :: 		INTCON        = 0xD8;
	MOVLW      216
	MOVWF      INTCON+0
;motorDriver.c,74 :: 		}
L_end_InitInterrupts:
	RETURN
; end of _InitInterrupts

_main:

;motorDriver.c,75 :: 		void main() {
;motorDriver.c,76 :: 		ADCON1 = 14;
	MOVLW      14
	MOVWF      ADCON1+0
;motorDriver.c,77 :: 		CMCON  = 7;
	MOVLW      7
	MOVWF      CMCON+0
;motorDriver.c,78 :: 		PORTB = 0;
	CLRF       PORTB+0
;motorDriver.c,79 :: 		TRISB = 243;
	MOVLW      243
	MOVWF      TRISB+0
;motorDriver.c,80 :: 		PORTD = 0;
	CLRF       PORTD+0
;motorDriver.c,81 :: 		TRISD = 0;
	CLRF       TRISD+0
;motorDriver.c,82 :: 		InitInterrupts();
	CALL       _InitInterrupts+0
;motorDriver.c,83 :: 		PWM1_Init(300);
	BSF        T2CON+0, 0
	BSF        T2CON+0, 1
	MOVLW      208
	MOVWF      PR2+0
	CALL       _PWM1_Init+0
;motorDriver.c,84 :: 		UART1_Init(9600);
	MOVLW      25
	MOVWF      SPBRG+0
	BSF        TXSTA+0, 2
	CALL       _UART1_Init+0
;motorDriver.c,87 :: 		while(1){
L_main10:
;motorDriver.c,88 :: 		PORTB.F2 = 1;
	BSF        PORTB+0, 2
;motorDriver.c,89 :: 		serialDataRead();
	CALL       _serialDataRead+0
;motorDriver.c,90 :: 		convertReceivedData();
	CALL       _convertReceivedData+0
;motorDriver.c,91 :: 		PORTB.F2 = 0;
	BCF        PORTB+0, 2
;motorDriver.c,92 :: 		readHallSensors();
	CALL       _readHallSensors+0
;motorDriver.c,93 :: 		updateMotorPins();
	CALL       _updateMotorPins+0
;motorDriver.c,94 :: 		PORTB.F2 = 0;
	BCF        PORTB+0, 2
;motorDriver.c,95 :: 		updatePWM();
	CALL       _updatePWM+0
;motorDriver.c,96 :: 		}
	GOTO       L_main10
;motorDriver.c,97 :: 		}
L_end_main:
	GOTO       $+0
; end of _main

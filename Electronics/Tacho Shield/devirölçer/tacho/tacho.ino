
int linePin = 11;
int tempPin = 0;

int milisecond=10; // the time it takes each reading
long milisecond_last=0;

int stat=0;
int stat_last;

int count=0;

double rpm=0;
String rpmStr;

double temp = 0;
String tempStr;

int nLine=3; // the number of blades of the propeller



void setup()
{
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  pinMode(linePin,INPUT)
	
}

void loop()
{
  
   readLineSensor();
   readTemperatureSensor();
   
   if(millis()-milisecond_last>=milisecond){
     rpm=((double)count/nLine)*600;
     rpmStr=String((int)rpm);
     Serial.print("*"+rpmStr+"/"+tempStr+"?");
     count=0;
     milisecond_last=millis();
   }
}


void readLineSensor(){
	
	stat = digitalRead(linePin);
	if(stat_last!=stat){
		
		count++;
		stat_last = stat;
	}
		
}

void readTemperatureSensor(){
	
	temp = analogRead(tempPin);
	tempStr = String((int)temp);
	
}
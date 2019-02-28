#define tachometerSerial1 Serial
#define tachometerSerial2 Serial1
#define motorDriverSerial1 Serial2
#define motorDriverSerial2 Serial3

int rpm1 = 0;
int rpm2 = 0;
int curr1 = 0;
int curr2 = 0;

void setup() {
tachometerSerial1.begin(50000);
tachometerSerial2.begin(50000);
motorDriverSerial1.begin(50000);
motorDriverSerial2.begin(50000);  
}

void loop() {
String rpmStr1 = String(rpm1);  
tachometerSerial1.print("*"+rpmStr1+"?");
rpm1++;
String rpmStr2 = String(rpm2);
tachometerSerial2.print("*"+rpmStr2+"?");
rpm2++;
String currStr1 = String(curr1);
motorDriverSerial1.print("*"+currStr1+"?");
curr1++;
String currStr2 = String(curr2);
motorDriverSerial2.print("*"+currStr2+"?");
curr2++;
delay(100);
}

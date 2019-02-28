void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
}
double k = 0;
double i = 0;
void loop() {
  // put your main code here, to run repeatedly:
k = map(i,0,1000,0,255);
String out = "*"+String(int(k))+"?";
Serial.print(out);
i++;
if(i>1000)
i = 0;
delay(100);
}

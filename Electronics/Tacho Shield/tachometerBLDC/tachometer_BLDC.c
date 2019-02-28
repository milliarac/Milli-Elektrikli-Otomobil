int numOfDetection = 0;

void Interrupt(){
if(RBIF_bit){
numOfDetection++;
}
}
void main() {
UART1_Init(9600);
while(){

}
}
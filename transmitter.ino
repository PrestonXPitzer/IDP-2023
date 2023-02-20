
void setup() {
  // put your setup code here, to run once:
  pinMode(3, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);

}
int baud = 22; //This global variable is the number of milliseconds we should transmit a frequency for, in this case 
// a baud rate of 45.45 converts direclty a a transmission time of 22 milliseconds. 
void sendSquare(int pin, int freq,int duty){
  //we want to send the square wave for a certain amount of time, so first find out what the timestamp is at the start of the loop
  int t_start = millis();
  while(millis()>(duty-t_start)){//Continue sending the data until the duty cycle is over
    int period = (1/freq)*.5;
    digitalWrite(pin, HIGH);
    delay(period);
    digitalWrite(pin, LOW);
    delay(period);
  }

  
}
int binToFreq(int value){
  if (value == 0){
    return 2295;
  }
  else return 2125;
}
void character_wise_transmission(char character, int pin){
  //Convert the integer char value into it's ascii equivalent, using the same process that we use to convert dec to bin by hand
  int bitwise[6] = {};
  for (int i = 8; i>0;i--){
    bitwise[i] = character%2;//if there is a remainder, then that bit is one
    character = character/2; //divide by 2
  }
  
  //Send the first low bit, at 45.45 baud, the duty cycle per bit is going to be .022 or 22ms always. 
  //For all transmissions 2125Hz = 0, 2295Hz = 1
  sendSquare(pin,2125,baud); //send 0 to start
  sendSquare(pin,binToFreq(bitwise[0]),baud); //The frequency of the square wave that we send varies on
  sendSquare(pin,binToFreq(bitwise[1]),baud); //the binary value of the character input. 
  sendSquare(pin,binToFreq(bitwise[2]),baud);
  sendSquare(pin,binToFreq(bitwise[3]),baud);
  sendSquare(pin,binToFreq(bitwise[4]),baud);
  sendSquare(pin,binToFreq(bitwise[5]),baud);
  sendSquare(pin,2295,baud); //The stop bit is always high 
  
  
  
}
void loop() {
  String hello = "Hello World!";
  // put your main code here, to run repeatedly:
  for (int i=0;i<hello.length()-1;i++){
    character_wise_transmission(hello[i],3);
  }
}

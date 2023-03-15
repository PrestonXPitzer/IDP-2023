
void setup() {
  // put your setup code here, to run once:
  pinMode(2, OUTPUT);
  Serial.begin(9600);
}
int baud = 22;
void sendSquare(int pin, int freq,int duty){
  //we want to send the square wave for a certain amount of time,
  //so first find out what the timestamp is at the start of the loop
  int period = (5000/freq);
  for( uint32_t tStart = millis();  (millis()-tStart) < duty;){
    digitalWrite(pin, HIGH);
    delay(period);
    digitalWrite(pin, LOW);
    delay(period);
  }
}
/////////////////////////////////////////////////////////////////////////////
unsigned int binToFreq(int value){
  if (value == 0){
    return 2295;
  }
  else return 2125;
}
//tone ( pin_number, frequency, duration );
/////////////////////////////////////////////////////////////////////////////////
void character_wise_transmission(char character, int pin){
  //Convert the integer char value into it's ascii equivalent, using the same process that we use to convert dec to bin by hand
  int bits[8];
  for (int i = 7; i >= 0; i--) {
    bits[i] = (character & (1 << i)) != 0;
  }
  //Send the first low bit, at 45.45 baud, the duty cycle per bit is going to be .022 or 22ms always. 
  //For all transmissions 2125Hz = 0, 2295Hz = 1
  sendSquare(pin,binToFreq(0),baud); //send 0 to start
  sendSquare(pin,binToFreq(bits[0]),baud); //The frequency of the square wave that we send varies on
  sendSquare(pin,binToFreq(bits[1]),baud); //the binary value of the character input. 
  sendSquare(pin,binToFreq(bits[2]),baud);
  sendSquare(pin,binToFreq(bits[3]),baud);
  sendSquare(pin,binToFreq(bits[4]),baud);
  sendSquare(pin,binToFreq(bits[5]),baud);
  sendSquare(pin,binToFreq(bits[6]),baud);
  sendSquare(pin,binToFreq(bits[7]),baud);
  sendSquare(pin,binToFreq(1),baud); //The stop bit is always high 
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  String hello = "g";
  for (int i = 0;i<hello.length();i++){
    character_wise_transmission(hello[i],2);
  }  // put your main code here, to run repeatedly:
  delay(22);
  
  
}

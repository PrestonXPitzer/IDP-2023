
void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  Serial.begin(9600);
}
int baud = 22;
void character_wise_transmission(char character, int pin){
  //Convert the integer char value into it's ascii equivalent, using the same process that we use to convert dec to bin by hand
  int frame[11]; //array to hold the hamming code modified 
  int bits[7];
  for (int i = 7; i >= 0; i--) {
    bits[i] = (character & (1 << i)) != 0;
  }
  //fill in the hamming frame with the character bits
  frame[10] = bits[6];
  frame[9] = bits[5];
  frame[8] = bits[4];
  frame[6] = bits[3];
  frame[5] = bits[2];
  frame[4] = bits[1];
  frame[2] = bits[0];
  frame[0] = (frame[2]^frame[4]^frame[6]^frame[8]^frame[10]) != 0;
  frame[1] = (frame[1]^frame[2]^frame[5]^frame[6]^frame[9]^frame[10]) != 0;
  frame[3] = (frame[3]^frame[4]^frame[5]^frame[6]) != 0;
  frame[7] = (frame[7]^frame[8]^frame[9]^frame[10]) != 0;
  //Send the first low bit, at 45.45 baud, the duty cycle per bit is going to be .022 or 22ms always. 
  //For all transmissions 2125Hz = 0, 2295Hz = 1
  digitalWrite(pin, 0);
  delay(11);
  for (int j = 10; j >= 0; j--) {
    digitalWrite(pin, frame[j]);
    Serial.print(frame[j]);
    delay(11);
  }
  Serial.println();
  digitalWrite(pin, 1);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (!Serial.available()){ //by default (i.e. no input, transmit 1)
    digitalWrite(3, 1);
  }
  String hello = Serial.readString(); //take the input from the serial monitor
  for (int i = 0, n = hello.length(); i < n; i++) {
    character_wise_transmission(hello[i],3);
    delay(30);
  }
  
}

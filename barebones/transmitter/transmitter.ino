
void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  Serial.begin(9600);
}
int baud = 22;
void character_wise_transmission(char character, int pin){
  //Convert the integer char value into it's ascii equivalent, using the same process that we use to convert dec to bin by hand
  bool bits[8];
  for (int i = 7; i >= 1; i--) {
    bits[i] = (character & (1 << i)) != 0;
  }
  //handles the parity bit 
  int posBits = 0;
  for (int k = 7; k >= 1; k--){
    if (bits[k] == 1){
      posBits++;
    }
  }
  if (posBits % 2 != 0){ //aka if the number of positive bits is not Even
    bits[0] = 1; //then the parity bit is 1 to make it even
  }
  else{
    bits[0] = 0;  
  }
  //Send the first low bit, at 45.45 baud, the duty cycle per bit is going to be .022 or 22ms always. 
  //For all transmissions 2125Hz = 0, 2295Hz = 1
  digitalWrite(pin, 0);
  delay(11);
  for (int j = 7; j >= 0; j--) {
    digitalWrite(pin, bits[j]);
    Serial.print(bits[j]);
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

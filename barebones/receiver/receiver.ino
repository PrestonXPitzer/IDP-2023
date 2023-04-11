#define THRESHOLD 700
#define PIN_NUM A5

int read_val = -1;
char test = 0;
char previous = 1, current = 1;

char pseudoDigiRead(int pin) {
  read_val = analogRead(PIN_NUM);
  if (read_val >= THRESHOLD) {
    return 1;
  } else {
    return 0;
  }
}
/**
 * debug function
 */
 void printByte(byte in){
  for (int i = 7; i >= 1; i--) {
    Serial.print((in & (1 << i)));
  }
 }
/**
 * determine if the incoming byte has a parity error
 * the parity bit is in the lsb position, and the parity is even
 * adidtionally, the function should return the byte modified to no
 * longer contain the parity bit
 */
byte checkParity(byte in){
  int oneBits = 0;
  int parity = (in & (1 << 0));
  byte withoutParity = in/2;
  for (int i = 0;i<8;i++){
    if ((withoutParity & (1<<i)) == 1){
      oneBits++;
    }
  }
  if ((oneBits + parity) % 2 == 0){
    printByte(in);
    printByte(withoutParity);
    Serial.println();
  }
  else {
    printByte(in);
    printByte(withoutParity);
    Serial.print("Error");
    Serial.println();
  }
}
/**
 * 
 */
char hammingDecode(bool frame[11]){
  bool R1 = (frame[0]^frame[2]^frame[4]^frame[6]^frame[8]^frame[10]);
  bool R2 = frame[1]^frame[2]^frame[5]^frame[6]^frame[9]^frame[10]);
  bool R4 = (frame[3]^frame[4]^frame[5]^frame[6]);
  bool R8 = (frame[7]^frame[8]^frame[9]^frame[10]);
  int error = (R1+(2*R2)+(4*R4)+(8*R8));
  if (error != 0){
    frame[error-1] = !frame[error-1];
  }
  char outputCharacter = (frame[2]+2*frame[4]+4*frame[5]+8*frame[6]+16*frame[8]+32*frame[9]+64*frame[10]);
  print(outputCharacter);
}
void setup() {
  pinMode(PIN_NUM, INPUT);
  Serial.begin(9600);
}
void loop() {
  bool frame[11];
  previous = current;
  current = pseudoDigiRead(PIN_NUM);
  if (current == 0 && previous == 1) {
    delay(15);
    for (int i = 10; i >= 0; i--) {
      char in = pseudoDigiRead(PIN_NUM);
      frame[i] = in;
      delay(11);
    }
    hammingDecode(test);
  }
}

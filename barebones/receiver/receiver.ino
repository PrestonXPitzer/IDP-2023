#define THRESHOLD 700
#define PIN_NUM A5

int read_val = -1;
char test = 0;
char previous = 1, current = 1;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 4;

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
char hammingDecode(int frame[11]){
  int R1 = (frame[0]^frame[2]^frame[4]^frame[6]^frame[8]^frame[10]); //check group 1
  int R2 = (frame[1]^frame[2]^frame[5]^frame[6]^frame[9]^frame[10]); //check group 2
  int R4 = (frame[3]^frame[4]^frame[5]^frame[6]); //check group 3
  int R8 = (frame[7]^frame[8]^frame[9]^frame[10]); //check group 4
  int error = (R1+(2*R2)+(4*R4)+(8*R8)); //the error location is the decimal representation
                                         //of the binary value formed by the parity bits. 
  if (error != 0){ //if the error exists
    if (frame[error-1] == 1){
      frame[error-1] = 0; //then correct it by flipping the bit
    }
    else {
      frame[error-1] = 1;
    }
  }
  char outputCharacter = (frame[2]+2*frame[4]+4*frame[5]+8*frame[6]+16*frame[8]+32*frame[9]+64*frame[10]);
  Serial.print(outputCharacter);
}

/**
 * setup the pins
 */
void setup() {
  pinMode(PIN_NUM, INPUT);
  Serial.begin(9600);
}

/**
 * mainloop
 */
void loop() {
  int frame[11];
  previous = current; //the last value from the loop should be our new previous
  current = pseudoDigiRead(PIN_NUM); //read a new value for comparison
  if (current == 0 && previous == 1) { //if we read a 1 to 0 transition
    lastDebounceTime = millis(); //start timing from that point
  if ((millis() - lastDebounceTime) > debounceDelay){//if the 0 doesn't go back to a 1
        delay(15-debounceDelay); //then we know that the signal is real
    for (int i = 10; i >= 0; i--) { //for each of the bits in the frame
      char in = pseudoDigiRead(PIN_NUM); //read the current input state
      frame[i] = in; //hold this data in the frame
      delay(11); //wait for the next bit
    }
  }
    hammingDecode(frame);
  }
}

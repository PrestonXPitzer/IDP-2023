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
    Serial.print(withoutParity);
  }
  else {
    Serial.print(withoutParity);
    Serial.print("[Error]");
  }
}
void setup() {
  pinMode(PIN_NUM, INPUT);
  Serial.begin(9600);
}
void loop() {
  previous = current;
  current = pseudoDigiRead(PIN_NUM);
  if (current == 0 && previous == 1) {
    delay(15);
    for (int i = 7; i >= 0; i--) {
      char in = pseudoDigiRead(PIN_NUM);
      test |= (in << i);
      delay(11);
    }
    checkParity(test);
  }
}

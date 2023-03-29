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

void setup() {
  pinMode(PIN_NUM, INPUT);
  Serial.begin(9600);
}
void loop() {
  previous = current;
  current = pseudoDigiRead(PIN_NUM);
  int bits[8];
  if (current == 0 && previous == 1) {
    delay(15);
    for (int i = 7; i >= 0; i--) {
      char in = pseudoDigiRead(PIN_NUM);
      bits[i] = in;
      test |= (in << i);
      delay(11);
    }
    //grab the parity bit seperately for easier calculation
    int parityBit = pseudoDigiRead(PIN_NUM);
    delay(11);
    //determine the number of 1 bits that we got
    int posBits = 0;
    for (int j = 0;j<8;j++){
      if (bits[j] == 1){
        posBits++;
      }
    }
    //if with the parity bit we have an even number than we're chillin
    //otherwise notify the user that we are not chillin
    if ((posBits + parityBit) % 2 != 0){ 
      Serial.print("[likely error]");
    }
    
    
    Serial.print(test);
    test = 0;
  }

}

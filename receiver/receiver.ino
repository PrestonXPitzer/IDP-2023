#include "hamming.h"

#define THRESHOLD 700
#define RECV_PIN A5
#define PACKET_SIZE 16
#define BAUD 45

const int period = (1000 / BAUD);
const int half_period = period / 2;

int read_val = -1;
char test = 0;
char previous = 1, current = 1;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 4;

char pseudoDigiRead(int pin) {
  read_val = analogRead(pin);
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

void setup() {
  pinMode(RECV_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  uint16_t packet = 0;
  char receive = 0;
  previous = current;
  current = pseudoDigiRead(RECV_PIN);
  if (current == 0 && previous == 1) {
    lastDebounceTime = millis(); //start timing from that point
  }
  if ((millis() - lastDebounceTime) > debounceDelay && current == 0) { //if the 0 doesn't go back to a 1
    delay((half_period * 3 / 2) - debounceDelay);  //then we know that the signal is real
    for (int i = (PACKET_SIZE - 1); i >= 0; i--) {
      packet |= (pseudoDigiRead(RECV_PIN) << i);
      delay(half_period);
    }
    receive = hamming_decode(packet);
    Serial.print(receive);
  }
}

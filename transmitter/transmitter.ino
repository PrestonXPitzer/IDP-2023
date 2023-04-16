#include "hamming.h"

#define TRAN_PIN 3
#define PACKET_SIZE 16
#define BAUD 45

const int period = (1000 / BAUD);
const int half_period = period / 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(TRAN_PIN,OUTPUT);
  Serial.begin(9600);
}
void character_wise_transmission(char character, int pin) {
  uint16_t packet = hamming_encode(character);

  //Convert the integer char value into it's ascii equivalent, using the same process that we use to convert dec to bin by hand
  bool bits[PACKET_SIZE]; //array to hold the character 
  for (int i = (PACKET_SIZE - 1); i >= 0; i--) {
    bits[i] = ((packet & (1 << i)) != 0);
  }

  //Send the first low bit, at 45.45 baud, the duty cycle per bit is going to be .022 or 22ms always. 
  //For all transmissions 2125Hz = 0, 2295Hz = 1
  digitalWrite(pin, 0);
  delay(half_period);
  for (int j = (PACKET_SIZE - 1); j >= 0; j--) {
    digitalWrite(pin, bits[j]);
    Serial.print(bits[j]);
    delay(half_period);
  }
  Serial.println();
  digitalWrite(pin, 1);
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
void loop() {
  if (!Serial.available()){ //by default (i.e. no input, transmit 1)
    digitalWrite(TRAN_PIN, 1);
  }
  String hello = Serial.readString(); //take the input from the serial monitor
  for (int i = 0, n = hello.length(); i < n; i++) {
    character_wise_transmission(hello[i], TRAN_PIN);
    delay(30);
  }
  
}

#ifndef HAMMING_H
#define HAMMING_H

#include <stdint.h>
#include <stdbool.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 


// Given character outputs two 7,4 Hamming encoded frames in series
uint16_t hamming_encode(char data);

// Given a packet containing two 7,4 Hamming encoded frames in series, outputs
// a character
char hamming_decode(uint16_t packet);

#endif

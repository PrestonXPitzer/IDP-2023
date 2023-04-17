#include "hamming.h"

static uint8_t parity_calc(uint8_t data);
static uint8_t hamming_organize(uint8_t data);
static uint8_t hamming_parity(uint8_t packet);

/**
 * encodes a character input 
 */
uint16_t hamming_encode(char data) {
    const uint8_t high_nyb = (data >> 4);  //make a new 8 bit packet with the top half
    const uint8_t low_nyb = (data & 0x0F); //mask off all but the bottom half

    uint8_t resultp1 = hamming_organize(high_nyb); //return a modified version of the data with parity bits
    uint8_t resultp2 = hamming_organize(low_nyb); //same for the bottom half
    uint16_t result = ((uint16_t) resultp1 << 8) | ((uint16_t) resultp2); //the result is both of these together
    return result;
}

/**
 * takes a 16 bit recieved data packet and returns the data inside
 */
char hamming_decode(uint16_t packet) {
    const uint8_t high_byte = ((uint8_t) (packet >> 8) & 0x00FF); //mask off the top 8 bits
    const uint8_t low_byte = ((uint8_t) packet & 0x00FF); // mask off the lower 8 bits
    uint8_t resultp1 = hamming_parity(high_byte); //get the corrected bits for the top
    uint8_t resultp2 = hamming_parity(low_byte); //and the corrected bits for the bottom

    char result = ((char) resultp1 << 4) | ((char) resultp2); //the result is these two together.
    return result; //return the character
}
/**
 * determines the appropriate parity value for the input data
 * @return the single bit parity value associated with the data
 * @param the input data exclusive of the parity bit
 */
static uint8_t parity_calc(uint8_t data) {
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 8; i++) { //for each element in the data input
        if ((data & (1 << (7 - i))) != 0) { //if the value at the index is 1
            parity ^= i; //bitwise xor with the current parity value (even parity)
        }
    }
    return parity; //return the calculated parity value
}
/**
 * rearranges the input data into a pattern such that we can add parity bits 
 */
static uint8_t hamming_organize(uint8_t data) {
    uint8_t result = ((data & 0x8) << 1) | (data & 0x7); // 00010111 mask
    uint8_t parity = parity_calc(result); //calculates parity on the masked data
    result |= ((parity & 0x4) << 1); //moves the parity bits into their appropriate location
    result |= ((parity & 0x2) << 4); //group 2
    result |= ((parity & 0x1) << 6); //group 3
    uint8_t parity_check = 0; //holds the parity value of the last group

    for (uint8_t i = 0; i < 8; i++) { //determines the value of the last parity bit
        parity_check ^= ((result & (1 << (7 - i))) != 0); 
    }

    result |= (parity_check << 7); //assign the last parity bit
    return result; //return the modified data frame
}
/*
 * checks the parity bits in the input packet and corrects for any error
 * that we find 
 */
static uint8_t hamming_parity(uint8_t packet) {
    uint8_t result = 0;

    uint8_t error = parity_calc(packet); //check the input packet for it's parity value
    if (error != 0) { //if it's nonzero then there is an error
        packet ^= (1 << (7 - error)); //so flip that bit 
    }

    result = ((packet & 0x10) >> 1) | (packet & 0x7); //mask off just the parity bits. 

    return result; //return just the pairty bits
}

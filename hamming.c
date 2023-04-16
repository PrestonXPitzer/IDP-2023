#include "hamming.h"

static uint8_t parity_calc(uint8_t data);
static uint8_t hamming_organize(uint8_t data);
static uint8_t hamming_parity(uint8_t packet);

uint16_t hamming_encode(char data) {
    const uint8_t high_nyb = (data >> 4);
    const uint8_t low_nyb = (data & 0x0F);

    uint8_t resultp1 = hamming_organize(high_nyb);
    uint8_t resultp2 = hamming_organize(low_nyb);
    uint16_t result = ((uint16_t) resultp1 << 8) | ((uint16_t) resultp2);
    return result;
}


char hamming_decode(uint16_t packet) {
    const uint8_t high_byte = ((uint8_t) (packet >> 8) & 0x00FF);
    const uint8_t low_byte = ((uint8_t) packet & 0x00FF);
    uint8_t resultp1 = hamming_parity(high_byte);
    uint8_t resultp2 = hamming_parity(low_byte);

    char result = ((char) resultp1 << 4) | ((char) resultp2);
    return result;
}

static uint8_t parity_calc(uint8_t data) {
    uint8_t parity = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if ((data & (1 << (7 - i))) != 0) {
            parity ^= i;
        }
    }
    return parity;
}

static uint8_t hamming_organize(uint8_t data) {
    uint8_t result = ((data & 0x8) << 1) | (data & 0x7);
    uint8_t parity = parity_calc(result);
    result |= ((parity & 0x4) << 1);
    result |= ((parity & 0x2) << 4);
    result |= ((parity & 0x1) << 6);
    uint8_t parity_check = 0;

    for (uint8_t i = 0; i < 8; i++) {
        parity_check ^= ((result & (1 << (7 - i))) != 0);
    }

    result |= (parity_check << 7);
    return result;
}

static uint8_t hamming_parity(uint8_t packet) {
    uint8_t result = 0;

    uint8_t error = parity_calc(packet);
    if (error != 0) {
        packet ^= (1 << (7 - error));
    }

    result = ((packet & 0x10) >> 1) | (packet & 0x7);

    return result;
}

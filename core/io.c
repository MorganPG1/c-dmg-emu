#include <stdint.h>
#include "gb.h"

uint8_t read_io(dmg_gameboy_t *gb, uint16_t addr) {
    switch (addr) {
        case 0x00: // joyp
            return 0xFF;
        case 0x04:
            return gb->div;
        case 0x05:
            return gb->tima;
        case 0x06:
            return gb->tma;
        case 0x07:
            return gb->tac;
        case 0x44:
            return 0x90;
        default:
            return 0;
    }
}

void write_io(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    switch (addr) {
        case 0x04:
            gb->div = val;
            break;
        case 0x05:
            gb->tima = val;
            break;
        case 0x06:
            gb->tma = val;
            break;
        case 0x07:
            gb->tac = val;
            break;
        default:
            break;
    }
}

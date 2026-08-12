#include <stdbool.h>
#include <stdint.h>
#include "gb.h"
static const uint8_t BIT_INDEXES[4] = {
    9, // FREQ 0 - bit 9 high (1024 T-Cycles)
    3, // FREQ 1 - bit 3 high (16 T-Cycles)
    5, // FREQ 2 - bit 5 high (64 T-Cycles)
    7  // FREQ 3 - bit 7 high (256 T-Cycles)
};

bool step_io(dmg_gameboy_t *gb, uint8_t cycles) {
    bool irq = false;
    uint8_t c;
    for (c=0; c<cycles; c++) {
        gb->master_counter++;
        gb->div = (gb->master_counter >> 8) & 0xFF;
        
        bool en = (gb->tac >> 2) & 0b1;
        uint8_t freq = (gb->tac) & 0b11;

        uint8_t bit_ind = BIT_INDEXES[freq];
        bool bit = (gb->master_counter >> bit_ind) & 0b1;

        if ((!bit) && (gb->prev_signal) && (en)) {
            gb->tima++;
            if (gb->tima == 0) {
                gb->tima = gb->tma;
                irq = true;
            }
        }
    }
    return irq;
}

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

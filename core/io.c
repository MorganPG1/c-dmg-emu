/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/cpu.h
 * 
 * C file for the generic I/O (e.g. serial, timer, etc)
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "gb.h"
#include "memory.h"
#include "ppu.h"
static const uint8_t BIT_INDEXES[4] = {
    9, // FREQ 0 - bit 9 high (1024 T-Cycles)
    3, // FREQ 1 - bit 3 high (16 T-Cycles)
    5, // FREQ 2 - bit 5 high (64 T-Cycles)
    7  // FREQ 3 - bit 7 high (256 T-Cycles)
};

void step_io(dmg_gameboy_t *gb, uint8_t cycles) {
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
                fire_interrupt(gb, INT_TIMER);
            }
        }

        gb->prev_signal = bit;
    }
}

uint8_t read_io(dmg_gameboy_t *gb, uint16_t addr) {
    switch (addr) {
        case 0x00: // joyp
            return ppu_poll_joyp(gb);
        case 0x04:
            return gb->div;
        case 0x05:
            return gb->tima;
        case 0x06:
            return gb->tma;
        case 0x07:
            return gb->tac;
        case 0xF:
            return gb->intf;
        case 0x40:
            return gb->lcdc;
        case 0x41:
            return 0x00;
        case 0x42:
            return gb->scy;
        case 0x43:
            return gb->scx;
        case 0x44:
            return gb->ly;
        case 0x45:
            return gb->lyc;
        case 0x4A:
            return gb->wy;
        case 0x4B:
            return gb->wx;
        default:
            return 0xFF;
    }
}

void write_io(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    switch (addr) {
        case 0x00:
            gb->joyp = val;
            break;
        case 0x01:
            gb->sb = val;
            break;
        case 0x02:
            if (val & 0x80 && !gb->debug) {
                putc(gb->sb, stdout);
                fflush(stdout);
            }
            break;
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
        case 0xF:
            gb->intf = val;
            break;
        case 0x40:
            gb->lcdc = val;
            break;
        case 0x42:
            gb->scy = val;
            break;
        case 0x43:
            gb->scx = val;
            break;
        case 0x45:
            gb->lyc = val;
            break;
        case 0x46: {
            uint16_t addr = (val << 8) & 0xDF00;
            int a;
            for (a=addr; a < addr+0xA0; a++) {
                gb->oam[a-addr] = read_mem_8b(gb, a);
            }
            break;
        }
        case 0x4A:
            gb->wy = val;
            break;
        case 0x4B:
            gb->wx = val;
            break;
        default:
            break;
    }
}

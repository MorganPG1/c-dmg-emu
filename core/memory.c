#include "memory.h"
#include "gb.h"
#include <stdint.h>

void write_mem_8b(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    return;
}

void write_mem_16b(dmg_gameboy_t *gb, uint16_t addr, uint16_t val) {
    uint8_t low = val & 0xFF;
    uint8_t high = (val >> 8) & 0xFF;
    write_mem_8b(gb, addr, low);
    write_mem_8b(gb, addr+1, high);
}

uint8_t read_mem_8b(dmg_gameboy_t *gb, uint16_t addr) {
    return 00;
}

uint16_t read_mem_16b(dmg_gameboy_t *gb, uint16_t addr) {
    uint8_t low = read_mem_8b(gb, addr);
    uint8_t high = read_mem_8b(gb, addr+1);

    uint16_t result = (high << 8) | low;
    return result;
}
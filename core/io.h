
#include "gb.h"
#include <stdint.h>

void step_io(dmg_gameboy_t *gb, uint8_t cycles);

uint8_t read_io(dmg_gameboy_t *gb, uint16_t addr);
void write_io(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);


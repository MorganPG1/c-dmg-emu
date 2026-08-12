
#include "gb.h"
#include <stdbool.h>
#include <stdint.h>

bool step_io(dmg_gameboy_t *gb, uint8_t cycles);

uint8_t read_io(dmg_gameboy_t *gb, uint16_t addr);
void write_io(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);


#include <stdbool.h>
#include <stdint.h>
#include "gb.h"

void ppu_init(dmg_gameboy_t *gb);
void ppu_step(dmg_gameboy_t *gb, uint8_t cycles);
void ppu_scan(dmg_gameboy_t *gb, bool obj_size, bool obj_en);
void ppu_render_scanline(dmg_gameboy_t *gb, bool bg_en, bool bg_tile_map_area, bool data_area, bool window_en, bool window_tile_map_area);
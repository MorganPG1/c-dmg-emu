#include "ppu.h"
#include "cpu.h"
#include "gb.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

#define WINDOW_W 640
#define WINDOW_H 576

#define GB_FB_W 160
#define GB_FB_H 144

#define CYCLES_MODE_0 204
#define CYCLES_MODE_1 456
#define CYCLES_MODE_2 80
#define CYCLES_MODE_3 172

// notes to self:
// framebuffer is in gb->fb
// it is an array of unsigned 32 bit integers
// each one is one pixel in RGBA
// you can find the index with (y * 160) + x

// update texture with SDL_UpdateTexture
// arg1 is the texture (gb->sdl_texture)
// arg2 is rect (NULL here)
// arg3 is framebuffer
// arg4 is bytes per row (GB_FB_W * 4)

// then ya do SDL_RenderClear to clear it
// SDL_RenderCopy to copy the texture to the renderer
// and then SDL_RenderPresent to display it

const char *window_title = "sdl window";
const uint32_t PALETTE[4] = {
    0xFFFFFFFF,
    0xD3D3D3FF,
    0xA9A9A9FF,
    0x000000FF
};

void ppu_init(dmg_gameboy_t *gb) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        GB_free(gb);
        exit(1);
    }
    gb->sdl_win = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    gb->sdl_renderer = SDL_CreateRenderer(gb->sdl_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gb->sdl_texture = SDL_CreateTexture(gb->sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, GB_FB_W, GB_FB_H);
}
void ppu_scan(dmg_gameboy_t *gb, bool obj_size, bool obj_en) {
    if (!obj_en) return;

    int obj;
    for (obj=0; obj<40; obj++) {
        uint16_t start_addr = (4 * obj);
        uint8_t y = gb->oam[start_addr];
        uint8_t x = gb->oam[start_addr + 1];
        uint8_t ind = gb->oam[start_addr + 2];
        uint8_t attr = gb->oam[start_addr + 3];
        int start_line = y - 16;

        uint8_t offset = 8;
        if (obj_size) offset = 16;    
            
        if (((start_line) <= (gb->ly)) && ((gb->ly) < (start_line + offset))) {
            gb->objs_on_line[gb->obj_c].y = y;
            gb->objs_on_line[gb->obj_c].x = x;
            gb->objs_on_line[gb->obj_c].tile_ind = ind;
            gb->objs_on_line[gb->obj_c].attr = attr;
            gb->objs_on_line[gb->obj_c].is_16px = obj_size;
            gb->obj_c++;
        } 
        if (gb->obj_c >= 10) {
            break;
        }
    }
}

void ppu_render_scanline(dmg_gameboy_t *gb, bool bg_en, bool bg_tile_map_area, bool data_area, bool window_en, bool window_tile_map_area) {
    return;
}
void ppu_step(dmg_gameboy_t *gb, uint8_t cycles) {
    bool lcd_en = (gb->lcdc & 0x80) != 0;
    bool window_tile_map_area = (gb->lcdc & 0x40) != 0;
    bool window_en = (gb->lcdc & 0x20) != 0;
    bool bg_data_area = (gb->lcdc & 0x10) != 0;
    bool bg_tile_map_area = (gb->lcdc & 0x8) != 0;
    bool obj_size = (gb->lcdc & 0x4) != 0;
    bool obj_en = (gb->lcdc & 0x2) != 0;
    bool bg_en = (gb->lcdc & 0x1) != 0;
    
    if (!lcd_en) {
        gb->ly = 0;
        gb->ppu_cycles = 0;
        gb->ppu_mode = PPU_HBLANK;
    }
    uint8_t c;
    for (c=0; c<cycles; c++) {
        gb->ppu_cycles++;
        switch (gb->ppu_mode) {
            case PPU_OBJ_SCAN: 
                if (gb->ppu_cycles == 1) {
                    gb->obj_c = 0;
                    ppu_scan(gb, obj_size, obj_en);
                } else if (gb->ppu_cycles >= CYCLES_MODE_2) {
                    gb->ppu_cycles = 0;
                    gb->ppu_mode = PPU_RENDERING;
                }
                break;
            case PPU_RENDERING:
                if (gb->ppu_cycles == 1) {
                    ppu_render_scanline(gb, bg_en, bg_tile_map_area, bg_data_area, window_en, window_tile_map_area);
                } else if (gb->ppu_cycles >= CYCLES_MODE_3) {
                    gb->ppu_cycles = 0;
                    gb->ppu_mode = PPU_HBLANK;
                }
                break;
            case PPU_HBLANK:
                if (gb->ppu_cycles >= CYCLES_MODE_0) {
                    gb->ppu_cycles = 0;
                    gb->ly++;
                    if (gb->ly < GB_FB_H) {
                        gb->ppu_mode = PPU_OBJ_SCAN;
                    } else {
                        gb->ppu_mode = PPU_VBLANK;
                        fire_interrupt(gb, INT_VBLANK);
                    }
                }
                break;
            case PPU_VBLANK:
                if (gb->ppu_cycles >= CYCLES_MODE_1) {
                    gb->ppu_cycles = 0;
                    gb->ly++;
                }
                if (gb->ly > 153) {
                    gb->ly = 0;
                    gb->ppu_mode = PPU_OBJ_SCAN;
                    gb->ppu_cycles = 0;

                    SDL_UpdateTexture(gb->sdl_texture, NULL, gb->fb, (GB_FB_W * 4));
                    SDL_RenderClear(gb->sdl_renderer);
                    SDL_RenderCopy(gb->sdl_renderer, gb->sdl_texture, NULL, NULL);
                    SDL_RenderPresent(gb->sdl_renderer);
                    
                    memset(gb->fb, 0, sizeof(gb->fb));
                    
                    
                }
                break;
        }
    }
    
    return;
}
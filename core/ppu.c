#include "ppu.h"
#include "gb.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdlib.h>

#define WINDOW_W 640
#define WINDOW_H 576

#define GB_FB_W 160
#define GB_FB_H 144

const char *window_title = "sdl window";

void init_ppu(dmg_gameboy_t *gb) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        GB_free(gb);
        exit(1);
    }
    gb->sdl_win = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
    gb->sdl_renderer = SDL_CreateRenderer(gb->sdl_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gb->sdl_texture = SDL_CreateTexture(gb->sdl_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, GB_FB_W, GB_FB_H);
}

void step_ppu(dmg_gameboy_t *gb) {
    return;
}
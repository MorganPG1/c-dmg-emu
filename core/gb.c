/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/gb.h
 * 
 * Main gameboy initialisation function
*/
#include "gb.h"
#include "rom.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>

dmg_gameboy_t* init_gb(bool debug, const char* rom_path) {
    dmg_gameboy_t* gb = malloc(sizeof(dmg_gameboy_t));
    if (gb == NULL) {
        return NULL;
    }

    gb->running = true;
    gb->ime = false;
    gb->halted = false;
    gb->halt_bug = false;
    gb->debug = debug;
    
    gb->cycles = 0;
    gb->intf = 0;
    gb->ie = 0;
    gb->ei_pending = 0;

    gb->af = 0x01B0;
    gb->bc = 0x0013;
    gb->de = 0x00D8;
    gb->hl = 0x014D;
    gb->sp = 0xFFFE;
    gb->pc = 0x0100;

    if (!init_rom(gb, rom_path)) {
        fprintf(stderr, "Unable to read rom file: %s\n", rom_path);
        free(gb);
        exit(1);
    }

    return gb;
}

void GB_log(const char *format, ...) {
    va_list args;

    va_start(args, format);
    vprintf(format, args);
}

void GB_log_err(const char *format, ...) {
    va_list args;

    va_start(args, format);
    vprintf(format, args);
}

void GB_stop_err(dmg_gameboy_t *gb, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    vprintf(format, args);
    gb->running = false;
}
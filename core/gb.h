/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/gb.h
 * 
 * Header file for the main gameboy struct and functions
*/

#ifndef GB_H
#define GB_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    union {
        uint16_t af;
        struct {
            uint8_t f;
            uint8_t a;
        };
    };

    union {
        uint16_t bc;
        struct {
            uint8_t c;
            uint8_t b;
        };
    };

    union {
        uint16_t de;
        struct {
            uint8_t e;
            uint8_t d;
        };
    };

    union {
        uint16_t hl;
        struct {
            uint8_t l;
            uint8_t h;
        };
    };

    uint16_t pc;
    uint16_t sp;
    uint8_t ei_pending;
    uint8_t intf;
    uint8_t ie;

    bool ime;
    bool running;
    bool halted;
    bool halt_bug;
} dmg_gameboy_t;

dmg_gameboy_t* init_gb(bool debug, const char* rom_path);
void GB_log(const char* format, ...);

#endif
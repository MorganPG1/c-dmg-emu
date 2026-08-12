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

#define VRAM_SIZE_BYTES 8192
#define WRAM_SIZE_BYTES 8192
#define HRAM_SIZE_BYTES 128
#define OAM_SIZE_BYTES 160
#define SRAM_SIZE_BYTES 131072 // allocate the most size used by a cartridge because thats the easiest way to do this

typedef enum {
    MBC0,
    MBC1,
    MBC2,
    MMM01,
    MBC3,
    MBC5,
    MBC6,
    MBC7,
    HuC3,
    HuC1
} mbc;

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

    unsigned char vram[VRAM_SIZE_BYTES];
    unsigned char wram[WRAM_SIZE_BYTES];
    unsigned char hram[HRAM_SIZE_BYTES];
    unsigned char oam[OAM_SIZE_BYTES];
    unsigned char sram[SRAM_SIZE_BYTES];
    unsigned char *rom;

    uint32_t cycles;
    uint16_t pc;
    uint16_t sp;
    uint8_t ei_pending;
    uint8_t intf;
    uint8_t ie;

    mbc mbc;
    uint8_t rom_bank;
    uint8_t sram_bank;
    long rom_size;
    
    char sb;
    
    uint8_t tima;
    uint8_t tma;
    uint8_t div;
    uint8_t tac;
    uint16_t master_counter;
    bool prev_signal;

    bool sram_en;
    bool ime;
    bool running;
    bool halted;
    bool halt_bug;
    bool debug;
} dmg_gameboy_t;

dmg_gameboy_t* init_gb(bool debug, const char* rom_path);
void GB_log(const char* format, ...);
void GB_log_err(const char* format, ...);
void GB_stop_err(dmg_gameboy_t *gb, const char* format, ...);
#endif
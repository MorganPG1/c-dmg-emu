/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/memory.h
 * 
 * Header file for the memory related functions
*/
#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include "gb.h"

typedef enum {
    REGION_ROM,
    REGION_VRAM,
    REGION_SRAM,
    REGION_WRAM,
    REGION_ECHO_WRAM,
    REGION_OAM,
    REGION_UNUSABLE,
    REGION_IO,
    REGION_HRAM,
    REGION_IE
} memory_region;

memory_region get_memory_region(dmg_gameboy_t *gb, uint16_t addr);

uint8_t read_mem_8b(dmg_gameboy_t *gb, uint16_t addr);
uint16_t read_mem_16b(dmg_gameboy_t *gb, uint16_t addr);

void write_mem_8b(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);
void write_mem_16b(dmg_gameboy_t *gb, uint16_t addr, uint16_t val);

#endif
/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/memory.h
 * 
 * Header file for the memory related functions
*/
#ifndef MEM_H
#define MEM_H

#include <cstdint>
#include <stdint.h>
#include "gb.h"

uint8_t read_mem_8b(dmg_gameboy_t *gb, uint16_t addr);
uint16_t read_mem_16b(dmg_gameboy_t *gb, uint16_t addr);

void write_mem_8b(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);
void write_mem_16b(dmg_gameboy_t *gb, uint16_t addr, uint16_t val);

#endif
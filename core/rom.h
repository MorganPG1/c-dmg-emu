/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/rom.h
 * 
 * Header file for the rom/mbc related functions
*/
#ifndef ROM_H
#define ROM_H

#include "gb.h"
#include <stdint.h>

void read_rom_metadata(dmg_gameboy_t *gb);

uint8_t read_mbc0(dmg_gameboy_t *gb, uint16_t addr);
uint8_t read_mbc1(dmg_gameboy_t *gb, uint16_t addr);

void write_mbc0(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);
void write_mbc1(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);

uint8_t read_rom(dmg_gameboy_t *gb, uint16_t addr);
void write_rom(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);
bool init_rom(dmg_gameboy_t *gb, const char* rom_path);

uint8_t read_sram(dmg_gameboy_t *gb, uint16_t addr);
void write_sram(dmg_gameboy_t *gb, uint16_t addr, uint8_t val);

#endif
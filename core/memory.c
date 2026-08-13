/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/memory.h
 * 
 * C file for the memory related functions
*/
#include "memory.h"
#include "gb.h"
#include "rom.h"
#include "io.h"
#include <linux/limits.h>
#include <stdint.h>

memory_region get_memory_region(dmg_gameboy_t *gb, uint16_t addr) {
    if (addr < 0x8000) {
        return REGION_ROM;
    } else if (addr < 0xA000) {
        return  REGION_VRAM;
    } else if (addr < 0xC000) {
        return REGION_SRAM;
    } else if (addr < 0xE000) {
        return REGION_WRAM;
    } else if (addr < 0xFE00) {
        return REGION_ECHO_WRAM;
    } else if (addr < 0xFEA0) {
        return REGION_OAM;
    } else if (addr < 0xFF00) {
        return REGION_UNUSABLE;
    } else if (addr < 0xFF80) {
        return REGION_IO;
    } else if (addr < 0xFFFF) {
        return REGION_HRAM;
    } else {
        return REGION_IE;
    }
}

uint16_t get_memory_offset(dmg_gameboy_t *gb, uint16_t addr, memory_region region) {
    switch (region) {
        case REGION_ROM:
            return addr;
        case REGION_VRAM:
            return addr-0x8000;
        case REGION_SRAM:
            return addr-0xA000;
        case REGION_WRAM:
            return addr-0xC000;
        case REGION_ECHO_WRAM:
            return addr-0xE000;
        case REGION_OAM:
            return addr-0xFE00;
        case REGION_UNUSABLE:
            return addr-0xFEA0;
        case REGION_IO:
            return addr-0xFF00;
        case REGION_HRAM:
            return addr-0xFF80;
        case REGION_IE:
            return addr-0xFFFF;
    }
}

void write_mem_8b(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    memory_region region = get_memory_region(gb, addr);
    uint16_t offset = get_memory_offset(gb, addr, region);
    switch (region) {
        case REGION_ROM:
            write_rom(gb, offset, val);
            break;
        case REGION_VRAM:
            gb->vram[offset] = val;
            break;
        case REGION_SRAM:
            write_sram(gb, offset, val);
            break;
        case REGION_WRAM:
            gb->wram[offset] = val;
            break;
        case REGION_ECHO_WRAM:
            gb->wram[offset] = val;
            break;
        case REGION_OAM:
            gb->oam[offset] = val;
            break;
        case REGION_UNUSABLE:
            break;
        case REGION_IO:
            write_io(gb, offset, val);
            break;
        case REGION_HRAM:
            gb->hram[offset] = val;
            break;
        case REGION_IE:
            gb->ie=val;
            break;
    }
}

void write_mem_16b(dmg_gameboy_t *gb, uint16_t addr, uint16_t val) {
    uint8_t low = val & 0xFF;
    uint8_t high = (val >> 8) & 0xFF;
    write_mem_8b(gb, addr, low);
    write_mem_8b(gb, addr+1, high);
}

uint8_t read_mem_8b(dmg_gameboy_t *gb, uint16_t addr) {
    memory_region region = get_memory_region(gb, addr);
    uint16_t offset = get_memory_offset(gb, addr, region);
    uint8_t result = 0;
    switch (region) {
        case REGION_ROM:
            result = read_rom(gb, offset);
            break;
        case REGION_VRAM:
            result = gb->vram[offset];
            break;
        case REGION_SRAM:
            result = read_sram(gb, addr);
            break;
        case REGION_WRAM:
            result = gb->wram[offset];
            break;
        case REGION_ECHO_WRAM:
            result = gb->wram[offset];
            break;
        case REGION_OAM:
            result = gb->oam[offset];
            break;
        case REGION_UNUSABLE:
            break;
        case REGION_IO:
            result = read_io(gb, offset);
            break;
        case REGION_HRAM:
            result = gb->hram[offset];
            break;
        case REGION_IE:
            result = gb->ie;
            break;
    }
    return result;
}

uint16_t read_mem_16b(dmg_gameboy_t *gb, uint16_t addr) {
    uint8_t low = read_mem_8b(gb, addr);
    uint8_t high = read_mem_8b(gb, addr+1);

    uint16_t result = (high << 8) | low;
    return result;
}
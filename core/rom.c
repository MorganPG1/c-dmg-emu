/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/rom.h
 * 
 * C file for the ROM/MBC related functions
*/
#include "rom.h"
#include "gb.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

void read_rom_metadata(dmg_gameboy_t *gb) {
    uint8_t cart_type = gb->rom[0x147];
    mbc cart_mbc = 0;
    if (cart_type == 0) {
        cart_mbc = MBC0;
    } else if (cart_type < 4) {
        cart_mbc = MBC1;
    } else {
        cart_mbc = MBC1;
    }
    gb->mbc = cart_mbc;
}

bool init_rom(dmg_gameboy_t *gb, char* rom_path) {
    FILE* ptr = fopen(rom_path, "rb");
    char* sram_path = strcat(rom_path, ".sav");

    FILE* tmp_sram_fp = fopen(sram_path, "rb");
    if (tmp_sram_fp != NULL) {
        fread(gb->sram, 1, sizeof(gb->sram), tmp_sram_fp);
        fclose(tmp_sram_fp);
    }
    

    if (ptr == NULL) {
        return false;
    }

    fseek(ptr, 0L, SEEK_END);
    long fsize = ftell(ptr);

    if (fsize <=0) {
        fclose(ptr);
        return false;
    }

    gb->rom = malloc(fsize);
    if (gb->rom == NULL) {
        fclose(ptr);
        return false;
    }

    fseek(ptr, 0L, SEEK_SET);
    size_t bytes_read = fread(gb->rom, 1, fsize, ptr);
    fclose(ptr);

    if (bytes_read != fsize) {
        free(gb->rom);
        return false;
    }
    
    gb->rom_size = fsize;
    read_rom_metadata(gb);

    gb->sram_fp = fopen(sram_path, "wb");
    if (gb->sram_fp == NULL) {
        return false;
    }
    fwrite(&gb->sram, 1,sizeof(gb->sram),gb->sram_fp);
    return true;
}

uint8_t read_mbc0(dmg_gameboy_t *gb, uint16_t addr) {
    return gb->rom[addr];
}

uint8_t read_mbc1(dmg_gameboy_t *gb, uint16_t addr) {
    if (addr < 0x4000) {
        return gb->rom[addr];
    } else {
        uint32_t phy_addr = (gb->rom_bank * 0x4000) + (addr - 0x4000);
        if (phy_addr < gb->rom_size) {
            return gb->rom[phy_addr];
        } else {
            return 0xFF;
        }
    }
}

void write_mbc0(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    return;
}

void write_mbc1(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    if ((addr >= 0x2000) && (addr < 0x4000)) {
        uint8_t bank = val;
        if (bank == 0) {
            gb->rom_bank = 1;
        } else {
            gb->rom_bank = bank;
        }
    } else if ((addr >= 0x4000) && (addr < 0x5FFF)) {
        uint8_t bank = val;
        gb->sram_bank = bank;
    }
}


uint8_t read_rom(dmg_gameboy_t *gb, uint16_t addr) {
    switch (gb->mbc) {
        case MBC0:
            return read_mbc0(gb, addr);
            break;    
        case MBC1:
            return read_mbc1(gb, addr);
            break;
        default:
            return read_mbc1(gb, addr);
    }
}

void write_rom(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    switch (gb->mbc) {
        case MBC0:
            write_mbc0(gb, addr, val);
            break;
        case MBC1:
            write_mbc1(gb, addr, val);
            break;
        default:
            return write_mbc1(gb, addr, val);
    }
}

uint8_t read_sram(dmg_gameboy_t *gb, uint16_t addr) {
    return gb->sram[(gb->sram_bank * 0x2000) + addr];
}

void write_sram(dmg_gameboy_t *gb, uint16_t addr, uint8_t val) {
    gb->sram[(gb->sram_bank * 0x2000) + addr] = val;
    fseek(gb->sram_fp, 0L, SEEK_SET);
    fwrite(gb->sram, 1,sizeof(gb->sram),gb->sram_fp);
}
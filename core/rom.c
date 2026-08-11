#include "rom.h"
#include "gb.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool init_rom(dmg_gameboy_t *gb, const char* rom_path) {
    FILE* ptr = fopen(rom_path, "rb");

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
    return true;
}
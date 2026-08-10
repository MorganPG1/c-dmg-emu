#include "gb.h"
#include "cpu.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t get_val_r8(dmg_gameboy_t *gb, uint8_t reg_idx) {
    switch (reg_idx) {
        case REG_B:
            return gb->b;
        case REG_C:
            return gb->c;
        case REG_D:
            return gb->d;
        case REG_E:
            return gb->e;
        case REG_H:
            return gb->h;
        case REG_L:
            return gb->l;
        case REG_MEM_HL:
            return read_mem_8b(gb, gb->hl);
        case REG_A:
            return gb->a;
        default:
            GB_stop_err(gb, "Unexpected register value passed to get_val_r8: %i", reg_idx);
            return 0;
    }
}

void set_val_r8(dmg_gameboy_t *gb, uint8_t reg_idx, uint8_t val) {
    switch (reg_idx) {
        case REG_B:
            gb->b = val;
            break;
        case REG_C:
            gb->c = val;
            break;
        case REG_D:
            gb->d = val;
            break;
        case REG_E:
            gb->e = val;
            break;
        case REG_H:
            gb->h = val;
            break;
        case REG_L:
            gb->l = val;
            break;
        case REG_MEM_HL:
            write_mem_8b(gb, gb->hl, val);
            break;
        case REG_A:
            gb->a = val;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to set_val_r8: %i", reg_idx);
            break;
    }
}

uint16_t get_val_r16(dmg_gameboy_t *gb, uint8_t reg_idx) {
    switch (reg_idx) {
        case REG_BC:
            return gb->bc;
        case REG_DE:
            return gb->de;
        case REG_HL:
            return gb->hl;
        case REG_SP:
            return gb->sp;
        default:
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16: %i", reg_idx);
            return 0;
    }
}

void set_val_r16(dmg_gameboy_t *gb, uint8_t reg_idx, uint16_t val) {
    switch (reg_idx) {
        case REG_BC:
            gb->bc = val;
            break;
        case REG_DE:
            gb->de = val;
            break;
        case REG_HL:
            gb->hl = val;
            break;
        case REG_SP:
            gb->sp = val;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16: %i", reg_idx);
            break;
    }
}

uint16_t get_val_r16stk(dmg_gameboy_t *gb, uint8_t reg_idx) {
    switch (reg_idx) {
        case REG_STK_BC:
            return gb->bc;
        case REG_STK_DE:
            return gb->de;
        case REG_STK_HL:
            return gb->hl;
        case REG_STK_AF:
            return gb->af;
        default:
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16stk: %i", reg_idx);
            return 0;
    }
}

void set_val_r16stk(dmg_gameboy_t *gb, uint8_t reg_idx, uint16_t val) {
    switch (reg_idx) {
        case REG_STK_BC:
            gb->bc = val;
            break;
        case REG_STK_DE:
            gb->de = val;
            break;
        case REG_STK_HL:
            gb->hl = val;
            break;
        case REG_STK_AF:
            gb->af = val;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16stk: %i", reg_idx);
            break;
    }
}

uint8_t get_val_r16mem(dmg_gameboy_t *gb, uint8_t reg_idx) {
    uint8_t val = 0;
    switch (reg_idx) {
        case REG_MEM_BC:
            val = read_mem_8b(gb, gb->bc);
            break;
        case REG_MEM_DE:
            val = read_mem_8b(gb, gb->de);
            break;
        case REG_MEM_HLI:
            val = read_mem_8b(gb, gb->hl);
            gb->hl++;
            break;
        case REG_MEM_HLD:
            val = read_mem_8b(gb, gb->hl);
            gb->hl--;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16mem: %i", reg_idx);
            break;
    }
    return val;
}

void set_val_r16mem(dmg_gameboy_t *gb, uint8_t reg_idx, uint8_t val) {
    switch (reg_idx) {
        case REG_MEM_BC:
            write_mem_8b(gb, gb->bc, val);
            break;
        case REG_MEM_DE:
            write_mem_8b(gb, gb->de, val);
            break;
        case REG_MEM_HLI:
            write_mem_8b(gb, gb->hl, val);
            gb->hl++;
            break;
        case REG_MEM_HLD:
            write_mem_8b(gb, gb->hl, val);
            gb->hl--;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16mem: %i", reg_idx);
            break;
    }
}

bool check_condition(dmg_gameboy_t *gb, uint8_t cond_idx) {
    uint8_t flags = gb->f;
    switch (cond_idx) {
        case COND_NZ:
            return (((flags > 7) & 0b1) != 0);
        case COND_Z:
            return (((flags > 7) & 0b1) == 0);
        case COND_NC:
            return (((flags > 4) & 0b1) != 0);
        case COND_C:
            return (((flags > 4) & 0b1) == 0);
        default:
            GB_stop_err(gb, "Unexpected condition value passed to check_condition: %i", cond_idx);
            return false;
    }
}

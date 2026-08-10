/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/gb.h
 * 
 * Header file for the main gameboy struct and functions
*/
#include <stdint.h>

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
} dmg_gameboy_t;

// Block 0
void handle_ld_r16_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_r16mem_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_a_r16mem(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_imm16_sp(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_inc_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_dec_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_add_hl_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_inc_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_dec_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_r8_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_flags_etc(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jr_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jrc_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_stop(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 1
void handle_ld_r8_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_halt(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 2 
void handle_alu_add(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_adc(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_sub(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_sbc(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_and(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_xor(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_or(dmg_gameboy_t *gb, uint8_t val);
void handle_alu_cp(dmg_gameboy_t *gb, uint8_t val);

void handle_alu_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 3
void handle_alu_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ret_cond(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ret(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_reti(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_cond_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_call_cond_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_call_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_rst(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_pop(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_push(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_prefix(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ldh_c_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ldh_imm8_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_imm16_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ldh_a_c(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ldh_a_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ldh_a_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_add_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_hl_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_sp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_di(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ei(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

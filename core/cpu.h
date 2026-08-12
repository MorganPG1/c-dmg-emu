/*
 * C DMG Gameboy Emulator Project - MorganPG
 * 
 * core/cpu.h
 * 
 * Header file for the CPU instruction handler functions
*/
#ifndef CPU_H
#define CPU_H

#include "gb.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    REG_B,
    REG_C,
    REG_D,
    REG_E,
    REG_H,
    REG_L,
    REG_MEM_HL,
    REG_A
} reg_r8;

typedef enum {
    REG_BC,
    REG_DE,
    REG_HL,
    REG_SP
} reg_r16;

typedef enum {
    REG_STK_BC,
    REG_STK_DE,
    REG_STK_HL,
    REG_STK_AF
} reg_r16stk;

typedef enum {
    REG_MEM_BC,
    REG_MEM_DE,
    REG_MEM_HLI,
    REG_MEM_HLD
} reg_r16mem;

typedef enum {
    COND_NZ,
    COND_Z,
    COND_NC,
    COND_C
} condition;

typedef enum {
    FLAG_ZERO,
    FLAG_SUB,
    FLAG_HALF_CARRY,
    FLAG_CARRY
} flag;

typedef enum {
    OPERATION_RLCA,
    OPERATION_RRCA,
    OPERATION_RLA,
    OPERATION_RRA,
    OPERATION_DAA,
    OPERATION_CPL,
    OPERATION_SCF,
    OPERATION_CCF
} flags_etc_operation;

typedef enum {
    ALU_ADD,
    ALU_ADC,
    ALU_SUB,
    ALU_SBC,
    ALU_AND,
    ALU_XOR,
    ALU_OR,
    ALU_CP
} alu_operation;

typedef enum {
    PREFIX_RLC,
    PREFIX_RRC,
    PREFIX_RL,
    PREFIX_RR,
    PREFIX_SLA,
    PREFIX_SRA,
    PREFIX_SWAP,
    PREFIX_SRL
} prefix_operation;

typedef enum {
    PREFIX_MAIN,
    PREFIX_BIT,
    PREFIX_RES,
    PREFIX_SET
} prefix_block;

// Helper functions
uint8_t read_imm8(dmg_gameboy_t *gb);
uint16_t read_imm16(dmg_gameboy_t *gb);

uint8_t get_val_r8(dmg_gameboy_t *gb, reg_r8 reg_idx);
void set_val_r8(dmg_gameboy_t *gb, reg_r8 reg_idx, uint8_t val);
uint16_t get_val_r16(dmg_gameboy_t *gb, reg_r16 reg_idx);
void set_val_r16(dmg_gameboy_t *gb, reg_r16 reg_idx, uint16_t val);
uint16_t get_val_r16stk(dmg_gameboy_t *gb, reg_r16stk reg_idx);
void set_val_r16stk(dmg_gameboy_t *gb, reg_r16stk reg_idx, uint16_t val);
uint8_t get_val_r16mem(dmg_gameboy_t *gb, reg_r16mem reg_idx);
void set_val_r16mem(dmg_gameboy_t *gb, reg_r16mem reg_idx, uint8_t val);
bool check_condition(dmg_gameboy_t *gb, condition cond_idx);

bool get_flag(dmg_gameboy_t *gb, flag flag_ind);
void set_flag(dmg_gameboy_t *gb, flag flag_ind);
void clear_flag(dmg_gameboy_t *gb, flag flag_ind);
void update_flag(dmg_gameboy_t *gb, flag flag_ind, bool condition);
// Instruction functions
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
void handle_jr_c_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_stop(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 1
void handle_ld_r8_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_halt(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 2 
uint16_t handle_alu_add(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_adc(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_sub(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_sbc(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_and(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_xor(dmg_gameboy_t *gb, uint8_t val);
uint8_t handle_alu_or(dmg_gameboy_t *gb, uint8_t val);

void handle_alu_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

// Block 3
void handle_alu_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ret_c(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ret(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_reti(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_c_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_jp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_call_c_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
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
void handle_ld_a_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_add_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_hl_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ld_sp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_di(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);
void handle_ei(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]);

void fire_interrupt(dmg_gameboy_t *gb, uint8_t interrupt);
void clear_interrupt(dmg_gameboy_t *gb, uint8_t interrupt);
void check_interrupt(dmg_gameboy_t *gb);
#endif
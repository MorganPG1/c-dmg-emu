#include "gb.h"
#include "cpu.h"
#include "memory.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t read_imm8(dmg_gameboy_t *gb) {
    uint8_t val = read_mem_8b(gb, gb->pc);
    
    if (!gb->halt_bug) {
        gb->pc++;
    } else {
        gb->halt_bug = false;
    }
    
    return val;
}

uint16_t read_imm16(dmg_gameboy_t *gb) {
    uint8_t low = read_imm8(gb);
    uint8_t high = read_imm8(gb);
    uint16_t val = low | (high << 8);
    return val;
}

uint8_t get_val_r8(dmg_gameboy_t *gb, reg_r8 reg_idx) {
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
            GB_stop_err(gb, "Unexpected register value passed to get_val_r8: %i\n", reg_idx);
            return 0;
    }
}

void set_val_r8(dmg_gameboy_t *gb, reg_r8 reg_idx, uint8_t val) {
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
            GB_stop_err(gb, "Unexpected register value passed to set_val_r8: %i\n", reg_idx);
            break;
    }
}

uint16_t get_val_r16(dmg_gameboy_t *gb, reg_r16 reg_idx) {
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
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16: %i\n", reg_idx);
            return 0;
    }
}

void set_val_r16(dmg_gameboy_t *gb, reg_r16 reg_idx, uint16_t val) {
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
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16: %i\n", reg_idx);
            break;
    }
}

uint16_t get_val_r16stk(dmg_gameboy_t *gb, reg_r16stk reg_idx) {
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
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16stk: %i\n", reg_idx);
            return 0;
    }
}

void set_val_r16stk(dmg_gameboy_t *gb, reg_r16stk reg_idx, uint16_t val) {
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
            gb->af = val & 0xFFF0;
            break;
        default:
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16stk: %i\n", reg_idx);
            break;
    }
}

uint8_t get_val_r16mem(dmg_gameboy_t *gb, reg_r16mem reg_idx) {
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
            GB_stop_err(gb, "Unexpected register value passed to get_val_r16mem: %i\n", reg_idx);
            break;
    }
    return val;
}

void set_val_r16mem(dmg_gameboy_t *gb, reg_r16mem reg_idx, uint8_t val) {
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
            GB_stop_err(gb, "Unexpected register value passed to set_val_r16mem: %i\n", reg_idx);
            break;
    }
}

bool check_condition(dmg_gameboy_t *gb, condition cond_idx) {
    uint8_t flags = gb->f;
    switch (cond_idx) {
        case COND_NZ:
            return (((flags >> 7) & 0b1) == 0);
        case COND_Z:
            return (((flags >> 7) & 0b1) == 1);
        case COND_NC:
            return (((flags >> 4) & 0b1) == 0);
        case COND_C:
            return (((flags >> 4) & 0b1) == 1);
        default:
            GB_stop_err(gb, "Unexpected condition value passed to check_condition: %i\n", cond_idx);
            return false;
    }
}

bool get_flag(dmg_gameboy_t *gb, flag flag_ind) {
    return (((gb->f >> (7-flag_ind)) & 0b1) == 1);
}

void set_flag(dmg_gameboy_t *gb, flag flag_ind) {
    gb->f |= (0x80 >> flag_ind);
}

void clear_flag(dmg_gameboy_t *gb, flag flag_ind) {
    uint8_t mask = ~(0x80 >> flag_ind);
    gb->f &= mask;
}

void update_flag(dmg_gameboy_t *gb, flag flag_ind, bool condition) {
    if (condition) {
        set_flag(gb, flag_ind);
    } else {
        clear_flag(gb, flag_ind);
    }
}
// Instruction functions
// Block 0
void handle_ld_r16_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t dest = (opcode >> 4) & 0b11;
    uint16_t val = read_imm16(gb);

    set_val_r16(gb, dest, val);
    gb->cycles += cycles[0];
}

void handle_ld_r16mem_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t dest = (opcode >> 4) & 0b11;
    set_val_r16mem(gb, dest, gb->a);
    
    gb->cycles += cycles[0];
}

void handle_ld_a_r16mem(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t source = (opcode >> 4) & 0b11;
    uint8_t val = get_val_r16mem(gb, source);
    
    gb->a = val;
    gb->cycles += cycles[0];
}

void handle_ld_imm16_sp(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_imm16(gb);
    write_mem_16b(gb, addr, gb->sp);

    gb->cycles += cycles[0];
}

void handle_inc_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t operand = (opcode >> 4) & 0b11;
    uint16_t val = get_val_r16(gb, operand) + 1;
    set_val_r16(gb, operand, val);

    gb->cycles += cycles[0];
}
void handle_dec_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t operand = (opcode >> 4) & 0b11;
    uint16_t val = get_val_r16(gb, operand) - 1;
    set_val_r16(gb, operand, val);

    gb->cycles += cycles[0];
}
void handle_add_hl_r16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t operand = (opcode >> 4) & 0b11;
    uint16_t val = get_val_r16(gb, operand);

    uint16_t result = val + gb->hl;

    update_flag(gb, FLAG_HALF_CARRY, (((gb->hl & 0xFFF) + (val & 0xFFF)) > 0xFFF));
    update_flag(gb, FLAG_CARRY, (result < gb->hl));
    clear_flag(gb, FLAG_SUB);

    gb->hl = result;
    gb->cycles += cycles[0];
}
void handle_inc_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t operand = (opcode >> 3) & 0b111;
    uint8_t val = get_val_r8(gb, operand);
    uint8_t result = val+1;

    update_flag(gb, FLAG_HALF_CARRY, (((val&0xF)+1) > 0xF));
    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);

    set_val_r8(gb, operand, result);
    gb->cycles += cycles[0];
}
void handle_dec_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t operand = (opcode >> 3) & 0b111;
    uint8_t val = get_val_r8(gb, operand);
    uint8_t result = val-1;

    update_flag(gb, FLAG_HALF_CARRY, ((val & 0xF) == 0));
    update_flag(gb, FLAG_ZERO, (result == 0));
    set_flag(gb, FLAG_SUB);

    set_val_r8(gb, operand, result);
    gb->cycles += cycles[0];
}
void handle_ld_r8_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t dest = (opcode>>3) & 0b111;
    uint8_t val = read_imm8(gb);

    set_val_r8(gb, dest, val);
    gb->cycles += cycles[0];
}
void handle_flags_etc(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    flags_etc_operation operation = (opcode >> 3) & 0b111;
    switch (operation) {
        case OPERATION_RLCA: {
            uint8_t result = (gb->a << 1) | (gb ->a >> 7);
            clear_flag(gb, FLAG_ZERO);
            clear_flag(gb, FLAG_SUB);
            clear_flag(gb, FLAG_HALF_CARRY);
            update_flag(gb, FLAG_CARRY, ((gb->a & 0x80) != 0));
            
            gb->a = result;
            break;
        }
        case OPERATION_RRCA: {
            uint8_t result = (gb->a >> 1) | (gb ->a << 7);
            clear_flag(gb, FLAG_ZERO);
            clear_flag(gb, FLAG_SUB);
            clear_flag(gb, FLAG_HALF_CARRY);
            update_flag(gb, FLAG_CARRY, ((gb->a & 0x01) != 0));
            
            gb->a = result;
            break;
        }
        case OPERATION_RLA: {
            uint8_t result = (gb->a << 1) | (get_flag(gb, FLAG_CARRY));
            clear_flag(gb, FLAG_ZERO);
            clear_flag(gb, FLAG_SUB);
            clear_flag(gb, FLAG_HALF_CARRY);
            update_flag(gb, FLAG_CARRY, ((gb->a & 0x80) != 0));
            
            gb->a = result;
            break;
        }
        case OPERATION_RRA: {
            uint8_t result = (gb->a >> 1) | (get_flag(gb, FLAG_CARRY) << 7);
            clear_flag(gb, FLAG_ZERO);
            clear_flag(gb, FLAG_SUB);
            clear_flag(gb, FLAG_HALF_CARRY);
            update_flag(gb, FLAG_CARRY, ((gb->a & 0x01) != 0));
            
            gb->a = result;
            break;
        }
        case OPERATION_DAA: {
            bool flag_n = get_flag(gb, FLAG_SUB);
            bool flag_hc = get_flag(gb, FLAG_HALF_CARRY);
            bool flag_c = get_flag(gb, FLAG_CARRY);
            uint8_t adj = 0;
            uint8_t result = gb->a;
            if (flag_n) {
                if (flag_hc) adj += 0x6;
                if (flag_c) adj += 0x60;
                result -= adj;
            } else {
                if ((flag_hc) || ((gb->a & 0xF) > 0x9)) adj += 0x6;
                if ((flag_c) || (gb->a > 0x99)) {
                    adj += 0x60;
                    set_flag(gb, FLAG_CARRY);
                }
                result += adj;
            }
            update_flag(gb, FLAG_ZERO, (result == 0));
            clear_flag(gb, FLAG_HALF_CARRY);

            gb->a = result;
            break;
        }
        case OPERATION_CPL:
            gb->a = ~gb->a;
            set_flag(gb, FLAG_SUB);
            set_flag(gb, FLAG_HALF_CARRY);
            break;
        case OPERATION_SCF:
            clear_flag(gb, FLAG_SUB);
            clear_flag(gb, FLAG_HALF_CARRY);
            set_flag(gb, FLAG_CARRY);
            break;
        case OPERATION_CCF:
            update_flag(gb, FLAG_CARRY, (!get_flag(gb,FLAG_CARRY)));
            break;
    }

    gb->cycles += cycles[0];
}
void handle_jr_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    int8_t val = read_imm8(gb);
    
    gb->pc += val;
    gb->cycles += cycles[0];
}
void handle_jr_c_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t condition = (opcode >> 3) & 0b11;
    int8_t val = read_imm8(gb);
    if (check_condition(gb, condition)) {
        gb->pc += val;
        gb->cycles += cycles[0];
    } else {
        gb->cycles += cycles[1];
    }
}
void handle_stop(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    GB_stop_err(gb, "STOP instruction (0x%02X) at PC %04X\n", opcode, gb->pc);
}

// Block 1
void handle_ld_r8_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    reg_r8 source = opcode & 0b111;
    reg_r8 dest = (opcode >> 3) & 0b111;

    uint8_t val = get_val_r8(gb, source);
    set_val_r8(gb, dest, val);

    gb->cycles += cycles[0];
}

void handle_halt(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    gb->halted = true;
    if (!gb->ime && gb->ei_pending){
        gb->halt_bug = true;
    }
    gb->cycles += cycles[0];
}

// Block 2 
uint16_t handle_alu_add(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t result = gb->a + val;

    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, (((gb->a & 0xF) + (val & 0xF)) > 0xF));
    update_flag(gb, FLAG_CARRY, (result < gb->a));

    return result;
}
uint8_t handle_alu_adc(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t carry = get_flag(gb, FLAG_CARRY);
    uint16_t full_result = gb->a + val + carry;
    uint8_t result = full_result & 0xFF;
    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, (((gb->a & 0xF) + (val & 0xF) + carry) > 0xF));
    update_flag(gb, FLAG_CARRY, (full_result > 0xFF));

    return result;
}
uint8_t handle_alu_sub(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t result = gb->a - val;

    update_flag(gb, FLAG_ZERO, (result == 0));
    set_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, ((gb->a & 0x0F) < (val & 0x0F)));
    update_flag(gb, FLAG_CARRY, (result > gb->a));

    return result;
}
uint8_t handle_alu_sbc(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t carry = get_flag(gb, FLAG_CARRY);
    uint16_t full_result = gb->a - (val + carry);
    uint8_t result = full_result & 0xFF;

    update_flag(gb, FLAG_ZERO, (result == 0));
    set_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, ((gb->a & 0x0F) < ((val &0x0F) + carry)));
    update_flag(gb, FLAG_CARRY, (full_result > 0xFFFF));

    return result;
}
uint8_t handle_alu_and(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t result = gb->a & val;

    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);
    set_flag(gb, FLAG_HALF_CARRY);
    clear_flag(gb, FLAG_CARRY);

    return result;
}
uint8_t handle_alu_xor(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t result = gb->a ^ val;

    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);
    clear_flag(gb, FLAG_HALF_CARRY);
    clear_flag(gb, FLAG_CARRY);

    return result;
}
uint8_t handle_alu_or(dmg_gameboy_t *gb, uint8_t val) {
    uint8_t result = gb->a | val;

    update_flag(gb, FLAG_ZERO, (result == 0));
    clear_flag(gb, FLAG_SUB);
    clear_flag(gb, FLAG_HALF_CARRY);
    clear_flag(gb, FLAG_CARRY);

    return result;
}

void handle_alu_r8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    reg_r8 operand = opcode & 0b111;
    alu_operation operation = (opcode >> 3) & 0b111;

    uint8_t val = get_val_r8(gb, operand);
    switch (operation) {
        case ALU_ADD:
            gb->a = handle_alu_add(gb, val);
            break;
        case ALU_ADC:
            gb->a = handle_alu_adc(gb, val);
            break;
        case ALU_SUB:
            gb->a = handle_alu_sub(gb, val);
            break;
        case ALU_SBC:
            gb->a = handle_alu_sbc(gb, val);
            break;
        case ALU_AND:
            gb->a = handle_alu_and(gb, val);
            break;
        case ALU_XOR:
            gb->a = handle_alu_xor(gb, val);
            break;
        case ALU_OR:
            gb->a = handle_alu_or(gb, val);
            break;
        case ALU_CP:
            handle_alu_sub(gb, val);
            break;
    }
    gb->cycles += cycles[0];
}

// Block 3
void handle_alu_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    alu_operation operation = (opcode >> 3) & 0b111;
    uint8_t val = read_imm8(gb);
    switch (operation) {
        case ALU_ADD:
            gb->a = handle_alu_add(gb, val);
            break;
        case ALU_ADC:
            gb->a = handle_alu_adc(gb, val);
            break;
        case ALU_SUB:
            gb->a = handle_alu_sub(gb, val);
            break;
        case ALU_SBC:
            gb->a = handle_alu_sbc(gb, val);
            break;
        case ALU_AND:
            gb->a = handle_alu_and(gb, val);
            break;
        case ALU_XOR:
            gb->a = handle_alu_xor(gb, val);
            break;
        case ALU_OR:
            gb->a = handle_alu_or(gb, val);
            break;
        case ALU_CP:
            handle_alu_sub(gb, val);
            break;
    }
    gb->cycles += cycles[0];
}
void handle_ret_c(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    condition cond = (opcode>>3) & 0b11;
    if (check_condition(gb, cond)) {
        uint16_t addr = read_mem_16b(gb, gb->sp);
        gb->pc = addr;
        gb->sp += 2;
        gb->cycles += cycles[0];
    } else {
        gb->cycles += cycles[1];
    }
}
void handle_ret(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_mem_16b(gb, gb->sp);
    gb->pc = addr;
    gb->sp += 2;
    
    gb->cycles += cycles[0];
}
void handle_reti(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_mem_16b(gb, gb->sp);
    gb->pc = addr;
    gb->sp += 2;
    gb->ime = true;

    gb->cycles += cycles[0];
}
void handle_jp_c_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    condition cond = (opcode >> 3) & 0b11;
    uint16_t addr = read_imm16(gb);
    if (check_condition(gb, cond)) {
        gb->pc = addr;
        gb->cycles += cycles[0];
    } else {
        gb->cycles += cycles[1];
    }
}
void handle_jp_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_imm16(gb);
    gb->pc = addr;
    gb->cycles += cycles[0];
}

void handle_jp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    gb->pc = gb->hl;
    gb->cycles += cycles[0];
}

void handle_call_c_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    condition cond = (opcode >> 3) & 0b11;
    uint16_t addr = read_imm16(gb);
    if (check_condition(gb, cond)) {
        write_mem_16b(gb, gb->sp-2, gb->pc);
        
        gb->sp -= 2;
        gb->pc = addr;
        gb->cycles += cycles[0];
    } else {
        gb->cycles += cycles[1];
    }
}

void handle_call_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_imm16(gb);
    write_mem_16b(gb, gb->sp-2, gb->pc);
    
    gb->sp -= 2;
    gb->pc = addr;
    gb->cycles += cycles[0];
}

void handle_rst(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = ((opcode >> 3) & 0b111) * 8;
    write_mem_16b(gb, gb->sp-2, gb->pc);
    
    gb->sp -= 2;
    gb->pc = addr;
    gb->cycles += cycles[0];
}

void handle_pop(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t data = read_mem_16b(gb, gb->sp);
    reg_r16stk operand = (opcode >> 4) & 0b11;

    set_val_r16stk(gb, operand, data);
    gb->sp += 2;
    gb->cycles += cycles[0];
}

void handle_push(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    reg_r16stk operand = (opcode >> 4) & 0b11;
    uint16_t data = get_val_r16stk(gb, operand);

    gb->sp -= 2;
    write_mem_16b(gb, gb->sp, data);
    gb->cycles += cycles[0];
}

void handle_prefix(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t prefix_opcode = read_imm8(gb);
    prefix_block blk = (prefix_opcode >> 6) & 0b11;
    reg_r8 operand = (prefix_opcode & 0b111);
    uint8_t ind = (prefix_opcode >> 3) & 0b111;
    uint8_t val = get_val_r8(gb, operand);
    uint8_t result = 0;
    switch (blk) {
        case PREFIX_MAIN: {
            prefix_operation op = ind;
            switch (op) {
                case PREFIX_RLC:
                    result = (val << 1) | (val >> 7);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x80) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }

                    break;
                case PREFIX_RRC:
                    result = (val >> 1) | (val << 7);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x01) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }

                    break;
                case PREFIX_RL:
                    result = (val << 1) | (get_flag(gb, FLAG_CARRY));
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x80) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }
                    
                    break;
                case PREFIX_RR:
                    result = (val >> 1) | (get_flag(gb, FLAG_CARRY) << 7);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x01) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }

                    break;
                case PREFIX_SLA:
                    result = (val << 1);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x80) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }

                    break;
                case PREFIX_SRA:
                    result = (val >> 1) | (val & 0x80);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x01) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }
                    break;
                case PREFIX_SWAP:
                    result = ((val >> 4) & 0xF) | ((val << 4) & 0xF0);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    clear_flag(gb, FLAG_CARRY);
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }
                    break;
                case PREFIX_SRL:
                    result = (val >> 1);
                    update_flag(gb, FLAG_ZERO, (result == 0));
                    clear_flag(gb, FLAG_SUB);
                    clear_flag(gb, FLAG_HALF_CARRY);
                    update_flag(gb, FLAG_CARRY, ((val & 0x01) != 0));
                    gb->cycles += 8;
                    if (operand == REG_MEM_HL) {
                        gb->cycles += 8;
                    }
                    break;
            }
            break;
        }
        case PREFIX_BIT: {
            uint8_t masked = val & (0b1 << ind);
            update_flag(gb, FLAG_ZERO, (masked == 0));
            clear_flag(gb, FLAG_SUB);
            set_flag(gb, FLAG_HALF_CARRY);
            
            gb->cycles += 8;
            if (operand == REG_MEM_HL) {
                gb->cycles += 4;
            }

            result = val;
            break;
        }
        case PREFIX_RES:
            result = val & ~(0b1 << ind);
            gb->cycles += 8;
            if (operand == REG_MEM_HL) {
                gb->cycles += 8;
            }
            break;
        case PREFIX_SET:
            result = val | (0b1 << ind);
            gb->cycles += 8;
            if (operand == REG_MEM_HL) {
                gb->cycles += 8;
            }
            break;
    }
    set_val_r8(gb, operand, result);
}

void handle_ldh_c_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = 0xFF00 + gb->c;

    write_mem_8b(gb, addr, gb->a);
    gb->cycles += cycles[0];
}

void handle_ldh_imm8_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t offset = read_imm8(gb);
    uint16_t addr = offset + 0xFF00;

    write_mem_8b(gb, addr, gb->a);
    gb->cycles += cycles[0];
}

void handle_ld_imm16_a(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_imm16(gb);

    write_mem_8b(gb, addr, gb->a);
    gb->cycles += cycles[0];
}

void handle_ldh_a_c(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = 0xFF00 + gb->c;

    gb->a = read_mem_8b(gb, addr);
    gb->cycles += cycles[0];
}

void handle_ldh_a_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint8_t offset = read_imm8(gb);
    uint16_t addr = offset + 0xFF00;

    gb->a = read_mem_8b(gb, addr);
    gb->cycles += cycles[0];
}

void handle_ld_a_imm16(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    uint16_t addr = read_imm16(gb);

    gb->a = read_mem_8b(gb, addr);
    gb->cycles += cycles[0];
}

void handle_add_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    int8_t val = read_imm8(gb);
    uint16_t result = gb->sp + val;

    clear_flag(gb, FLAG_ZERO);
    clear_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, (((val & 0xF) + (gb->sp & 0xF)) > 0xF));
    update_flag(gb, FLAG_CARRY, (((gb->sp & 0xFF) + (val & 0xFF)) > 0xFF));

    gb->sp = result;
    gb->cycles += cycles[0];
}

void handle_ld_hl_sp_imm8(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    int8_t val = read_imm8(gb);
    uint16_t result = gb->sp + val;

    clear_flag(gb, FLAG_ZERO);
    clear_flag(gb, FLAG_SUB);
    update_flag(gb, FLAG_HALF_CARRY, (((val & 0xF) + (gb->sp & 0xF)) > 0xF));
    update_flag(gb, FLAG_CARRY, (((gb->sp & 0xFF) + (val & 0xFF)) > 0xFF));

    gb->hl = result;
    gb->cycles += cycles[0];
}

void handle_ld_sp_hl(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    gb->sp = gb->hl;
    gb->cycles += cycles[0];
}

void handle_di(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    gb->ime = 0;
    gb->ei_pending = 0;
}

void handle_ei(dmg_gameboy_t *gb, uint8_t opcode, uint8_t cycles[2]) {
    gb->ei_pending = 2;
}

void fire_interrupt(dmg_gameboy_t *gb, uint8_t interrupt) {
    gb->intf |= (1 << interrupt);
}
void clear_interrupt(dmg_gameboy_t *gb, uint8_t interrupt) {
    gb->intf &= ~(1 << interrupt);
}

void check_interrupt(dmg_gameboy_t *gb) {
    uint8_t ints = gb->ie & gb->intf;
    uint8_t i;
    for (i=0; i<5; i++) {
        if ((ints & (1 << i)) != 0) {
            if (gb->halted) {
                gb->halted = false;
                gb->cycles += 4;
            }

            if (gb->ime) {
                clear_interrupt(gb, i);
                uint16_t handler = 0x40 + (0x8 * i);

                write_mem_16b(gb, gb->sp-2, gb->pc);
                gb->sp -= 2;
                gb->pc = handler;

                gb->ime = 0;
                gb->cycles += 20;
            }
        } 
    }
}


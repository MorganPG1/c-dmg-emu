#include "core/cpu.h"
#include "core/gb.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include "core/io.h"
#include "core/boilerplate.h"
#include "core/memory.h"

#define CYCLES_PER_FRAME 70224
#define TARGET_FRAME_TIME_NS 16742706

static dmg_gameboy_t *global_gb = NULL;
static uint64_t total_cycles = 0;
uint64_t get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void clean_exit(dmg_gameboy_t *gb) {
    if (gb->rom) {
        free(gb->rom);
    }
    free(gb);
}

void handle_exitsig(int sig) {
    if (global_gb) {
        clean_exit(global_gb);
    }
    printf("Total cycles: %lu\n", total_cycles);
    exit(0);
}

void step(dmg_gameboy_t *gb) {
    check_interrupt(gb);

    uint8_t opcode = read_imm8(gb);
    gb->cycles += 4;
    step_io(gb, gb->cycles);
    execute_instr(gb, opcode);
    step_io(gb, gb->cycles-4);
    if (gb->ei_pending != 0) {
        gb->ei_pending--;
        if (gb->ei_pending == 0) gb->ime = true;
    }
}

void mainloop(dmg_gameboy_t *gb) {
    uint64_t f_start = get_time_ns();
    while (gb->running) {
        uint32_t t_cycles = 0;
        while (t_cycles < CYCLES_PER_FRAME) {
            if (gb->debug) {
                uint16_t pc = gb->pc;
                GB_log("A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X IME:%i IF:%i IE:%i TAC:%02X TIMA:%02X INT_COUNT:%04X HALTED:%i CYCLE:%i\n", gb->a, gb->f, gb->b, gb->c, gb->d, gb->e, gb->h, gb->l, gb->sp, pc, read_mem_8b(gb, pc), read_mem_8b(gb, pc+1), read_mem_8b(gb, pc+2),read_mem_8b(gb, pc+3), gb->ime, gb->intf, gb->ie, gb->tac, gb->tima, gb->master_counter, gb->halted, total_cycles);
            }
            gb->cycles = 0;
            if (!gb->halted) {
                step(gb);
            } else {
                check_interrupt(gb);
                gb->cycles += 4;
                step_io(gb, gb->cycles);
            }
            t_cycles += gb->cycles;
            total_cycles += gb->cycles;
            
            
        }

        uint64_t f_end = get_time_ns();
        uint64_t elapsed = f_end - f_start;

        if (elapsed < TARGET_FRAME_TIME_NS && !gb->debug) {
            struct timespec sleep_time;
            uint64_t remaining = TARGET_FRAME_TIME_NS - elapsed;

            sleep_time.tv_sec =  remaining / 1000000000ULL;
            sleep_time.tv_nsec = remaining % 1000000000ULL;
            
            nanosleep(&sleep_time, NULL);
        }

        f_start = get_time_ns();
    }
}

int main( int argc, char** argv ) {
    bool debug;
    if (argc > 2) {
        debug = true;
    } else {
        debug = false;
    }
    dmg_gameboy_t *gb = init_gb(debug, argv[1]);
    
    
    global_gb = gb;
    signal(SIGINT, handle_exitsig);
    signal(SIGTERM, handle_exitsig);

    mainloop(gb);
    GB_log("Exiting, gb->running was set to false\n");
    clean_exit(gb);
}


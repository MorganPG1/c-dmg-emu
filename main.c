#include "core/cpu.h"
#include "core/gb.h"
#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include "core/io.h"
#include "core/boilerplate.h"
#include "core/memory.h"
#include "core/ppu.h"
#include <SDL2/SDL.h>

#define CYCLES_PER_FRAME 70224
#define TARGET_FRAME_TIME_NS 16742706

static dmg_gameboy_t *global_gb = NULL;
static uint64_t total_cycles = 0;
uint64_t get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}


void handle_exitsig(int sig) {
    if (global_gb) {
        global_gb->running = false;
    };
    printf("\nTotal cycles: %lu\n", total_cycles);
}

void step(dmg_gameboy_t *gb) {
    check_interrupt(gb);

    uint8_t opcode = read_imm8(gb);
    gb->cycles += 4;
    step_io(gb, gb->cycles);
    execute_instr(gb, opcode);
    step_io(gb, gb->cycles-4);
    ppu_step(gb, gb->cycles);

    if (gb->ei_pending != 0) {
        gb->ei_pending--;
        if (gb->ei_pending == 0) gb->ime = true;
    }
}

void mainloop(dmg_gameboy_t *gb) {
    uint64_t f_start = get_time_ns();
    while (gb->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    handle_exitsig(0);
                case SDL_KEYDOWN:
                    fire_interrupt(gb, INT_JOYPAD);
            }
            
        }
        
        if (gb->debug) {
            uint16_t pc = gb->pc;
            GB_log("A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X IME:%i IF:%i IE:%i TAC:%02X TIMA:%02X INT_COUNT:%04X HALTED:%i LCDC:%i LY:%i PPU_C:%i CYCLE:%i\n", gb->a, gb->f, gb->b, gb->c, gb->d, gb->e, gb->h, gb->l, gb->sp, pc, read_mem_8b(gb, pc), read_mem_8b(gb, pc+1), read_mem_8b(gb, pc+2),read_mem_8b(gb, pc+3), gb->ime, gb->intf, gb->ie, gb->tac, gb->tima, gb->master_counter, gb->halted, gb->lcdc, gb->ly, gb->ppu_cycles, total_cycles);
        }

        gb->cycles = 0;
        if (!gb->halted) { // this is a really stupid way to do this i'll redo this when i feel like it
            step(gb);
        } else {
            check_interrupt(gb);
            gb->cycles += 4;
            step_io(gb, gb->cycles);
            ppu_step(gb, gb->cycles);
        }

        total_cycles += gb->cycles;
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
    GB_log("Exiting.\n");
    GB_free(gb);
}


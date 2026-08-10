
#include "core/cpu.h"
#include "core/gb.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "core/boilerplate.h"

#define CYCLES_PER_FRAME 70224
#define TARGET_FRAME_TIME_NS 16742706

uint64_t get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

void mainloop(dmg_gameboy_t *gb) {
    uint64_t f_start = get_time_ns();

    while (gb->running) {
        uint32_t t_cycles = 0;
        while (t_cycles < CYCLES_PER_FRAME) {
            gb->cycles = 0;
            if (!gb->halted) {
                uint8_t opcode = read_imm8(gb);
                gb->cycles += 4;
                execute_instr(gb, opcode);
            } else {
                gb->cycles += 4;
            }
            t_cycles += gb->cycles;
        }

        uint64_t f_end = get_time_ns();
        uint64_t elapsed = f_end - f_start;

        if (elapsed < TARGET_FRAME_TIME_NS) {
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
    dmg_gameboy_t *gb = init_gb(true, argv[1]);
    mainloop(gb);
    free(gb);
}


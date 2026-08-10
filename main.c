
#include "core/gb.h"
#include <stdbool.h>
#include <stdlib.h>

void mainloop(dmg_gameboy_t *gb) {
    
}

int main( int argc, char** argv ) {
    dmg_gameboy_t *gb = init_gb(true, argv[1]);
    mainloop(gb);
    free(gb);
}


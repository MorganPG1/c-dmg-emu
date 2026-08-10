
#include "core/gb.h"
#include <stdbool.h>

int main( int argc, char** argv ) {
    dmg_gameboy_t *gb = init_gb(true, argv[1]);
}
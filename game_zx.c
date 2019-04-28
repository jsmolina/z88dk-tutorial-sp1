#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

// reversed order digit extract
void print_points(uint16_t number) {
    uint8_t pos = 18;

    while (number > 0)
    {
        number = number % 10;
        number /= 10;
        //print digit
        sp1_PrintAtInv(0, pos, INK_CYAN | PAPER_BLACK, 48 + number);
        --pos;
    }
}
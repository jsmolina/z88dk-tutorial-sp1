#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include "globals.h"

extern uint8_t strlen(char * chars);
// https://arda.kisafilm.org/fish/ay8910-2.htm
// temporary buffer to print points (e.g. 65535)
char * chars = "0000000\0";


void printatstr(uint8_t y, uint8_t x, uint8_t attr, char *chars) {

    uint8_t xx, i;
    xx = x; i=0;
    while (chars[i]) {
        sp1_PrintAtInv(y, xx++, attr, chars[i++]);
    }
}

// reversed order digit extract
void print_points() {
    utoa(points, chars, 10);
    col = 5 - strlen(chars);

    if(col != 0) {
        for(idx = 0; idx != 5; ++idx) {
            sp1_PrintAtInv(0, 25 + idx, INK_CYAN | PAPER_BLACK, '0');
        }
    }
    idx = 0;
    while(chars[idx] != '\0') {
        sp1_PrintAtInv(0, 25 + idx + col, INK_CYAN | PAPER_BLACK, chars[idx]);
        ++idx;
    }

}

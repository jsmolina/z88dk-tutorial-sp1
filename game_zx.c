#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include "globals.h"

extern uint8_t strlen(char * chars);
// https://arda.kisafilm.org/fish/ay8910-2.htm
// temporary buffer to print points (e.g. 65535)
char * chars = "0000000\0";

void make_sound() {
    __asm
      .ayctrl  EQU     65533
      .aydata  EQU     49149

                ld      d,0             ; select the register
                ld      e,10            ; set value
                call    outer           ; send it to PSG
                ld      d,1             ;
                ld      e,10            ;
                call    outer           ;
                ld      d,6             ;
                ld      e,29            ;
                call    outer           ;

                ld      d,7             ;
                ld      e,216            ;
                call    outer           ;

                ld      d,8             ;
                ld      e,31            ;
                call    outer           ;

                ld      d,11             ;
                ld      e,4            ;
                call    outer           ;

                ld      d,12             ;
                ld      e,4            ;
                call    outer           ;

                ret

        .outer   ld      bc,ayctrl       ; select control port
                out     (c),d           ; send specified value
                ld      bc,aydata       ; select data port
                out     (c),e           ; send specified value
                ret

    __endasm;
}

// reversed order digit extract
void print_points() {
    utoa(points, chars, 10);
    col = 5 - strlen(chars);

    if(col != 0) {
        for(idx = 0; idx != 5; ++idx) {
            sp1_PrintAtInv(0, 26 + idx, INK_CYAN | PAPER_BLACK, '0');
        }
    }
    idx = 0;
    while(chars[idx] != '\0') {
        sp1_PrintAtInv(0, 26 + idx + col, INK_CYAN | PAPER_BLACK, chars[idx]);
        ++idx;
    }
    make_sound();

}
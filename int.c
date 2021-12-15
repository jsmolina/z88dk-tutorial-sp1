#include <z80.h>
#include "int.h"
#include "globals.h"
#include <stdlib.h>
#include <intrinsic.h>

unsigned char tick;
unsigned char timer;
unsigned char pick;



void
wait(void)
{
   while (abs(tick - timer) < SPEED) {
      intrinsic_halt();
      in = (joy)(&joy_keys);
   }

   timer = tick;
}


void start_ay() {
__asm
    extern startLoad
    extern enable_bank_n
    extern restore_bank_0
    ld a,6
    call enable_bank_n
     call startLoad
    call restore_bank_0         ; bank 0 in top 16k, stack restored
__endasm;
}

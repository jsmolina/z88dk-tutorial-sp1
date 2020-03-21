#include <sound/aywyz.h>
#include <im2.h>
#include <stdlib.h>
#include <string.h>
#include <z80.h>
#include "globals.h"
#include "int.h"


unsigned char tick;
unsigned char timer;
unsigned char pick;


IM2_DEFINE_ISR(isr)
{
   // update the clock
   ++tick;
   ay_wyz_play();
}


void setup_int() {
   im2_init((void *)0xd000); // CRT_ORG = 25124
   memset((void *)0xd000, 0xd1, 257);

   z80_bpoke(0xd1d1, 0xc3);
   z80_wpoke(0xd1d2, (unsigned int)isr);

}


void
wait(void)
{
   while (abs(tick - timer) < SPEED) {
      intrinsic_halt();
      in = (joy)(&joy_keys);
   }

   timer = tick;
}

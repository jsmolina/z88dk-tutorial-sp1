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
   while (abs(tick - timer) < speed) {
      intrinsic_halt();
      in = (joy)(&joy_keys);
   }

   timer = tick;
}

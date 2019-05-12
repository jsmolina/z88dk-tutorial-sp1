#include <z80.h>
#include "int.h"
#include <stdlib.h>
#include <intrinsic.h>

unsigned char tick;
unsigned char timer;
unsigned char pick;



void
wait(void)
{
   while (abs(tick - timer) < WFRAMES)
      intrinsic_halt();

   timer = tick;
}

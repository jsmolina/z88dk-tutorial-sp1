#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <sound.h> // for bit_beepfx()
#include <string.h>
#include <stdlib.h>
#include <intrinsic.h>

// screen rectangle
struct sp1_Rect full_screen = {0, 0, 32, 24};
// it comes from built binaries:
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];
extern uint8_t sprite_protar3[];
extern uint8_t sprite_protar4[];

extern uint8_t cartoon0[];


void show_intro() {
    /**
    Switches to BANK 6 and then copies a SCREEN$ file directly to memory
    */
    __asm
    extern enable_bank_n
   di
   ld a,0x80
   ld i,a                      ; point I at uncontended bank

   ld a,6
   call enable_bank_n          ; bank 6 in top 16k, stack moved
    __endasm;
    memcpy(16384, cartoon0, 6900);
    __asm
    extern restore_bank_0
    call restore_bank_0
    ei
    __endasm;

}

static void initialiseColour(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_BLACK;
}

struct sp1_ss * add_sprite_misifu() {
  struct sp1_ss * sp;
   sp = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar2, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar3, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar4, 1);

  sp1_AddColSpr(sp, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColour);

  return sp;
}


int main()
{
  // show paging capabilities.
  show_intro();
  in_wait_key();

  // now sp1
  struct sp1_ss* sp = add_sprite_misifu();
  zx_border(INK_WHITE);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  ' ' );
  sp1_Invalidate(&full_screen);

  sp1_UpdateNow();

  while(1) {
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(sp, &full_screen, (void*) 1, 10, 12, 0, 0);
     sp1_UpdateNow();
  }
}

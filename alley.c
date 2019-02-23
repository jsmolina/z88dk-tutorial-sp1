#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <sound.h> // for bit_beepfx()
#include <string.h>

// screen rectangle
struct sp1_Rect full_screen = {0, 0, 32, 24};
// it comes from built binaries:
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];
extern uint8_t sprite_protar3[];
extern uint8_t sprite_protar4[];

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

/**
void page(unsigned char bank) {
//CAUTION BEAUTIFER SCEWS ALL!!!!!
  if (bank == 0) {
    __asm
    ld a, 0xFE
    ld i, a
    __endasm;
  } else {
    __asm
    ld a, 0
    ld i, a
    __endasm;
  }

  GLOBAL_ZX_PORT_7FFD = 0x10 + bank;
  IO_7FFD = 0x10 + bank;
}
void game_img( unsigned char *f_img, unsigned char f_page,
               unsigned char f_lin, unsigned char f_col,
               unsigned char f_width, unsigned char f_height) {

  unsigned char i;
  unsigned char *src;
  unsigned char *dest;
  unsigned int len;
  len = f_width * f_height;
  // 8 x 4 = 32 logo1
  // Read Draw
  NIRVANAP_halt(); /// DONT REMOVE CAUSE HANG!!!!
  intrinsic_di();
  i = 0;
  while (i < :heart_eyes: {
    NIRVANAP_spriteT(i, 0, 0, 0);
    ++i;
  }

  src = f_img;
  page(f_page);
  dest = &btiles[0];
  memcpy(dest, src, 48 * len);
  page(0);

  i = 0;
  s_col1 = f_col;
  s_lin1 = f_lin;

  while (i < (len)) {
    NIRVANAP_drawT_raw(i, s_lin1, s_col1 * 2);
    ++i;
    ++s_col1;
    if ((i % f_width) == 0) {
      s_lin1 = s_lin1 + 16;
      s_col1 = f_col;
    }
  }
  intrinsic_ei();

}
una función page pagina la memoria
y la otra dibuja la parte que necesitas es
src = f_img;
  page(f_page);
  dest = &btiles[0];
  memcpy(dest, src, 48 * len);
  page(0);


**/
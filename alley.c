#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <sound.h> // for bit_beepfx()
#include <string.h>
#include <stdlib.h>
#include <intrinsic.h>
#include <input.h>

#define RIGHTC1 1
#define RIGHTC2 33
#define LEFTC1 65
#define LEFTC2 87

// screen rectangle
struct sp1_Rect full_screen = {0, 0, 32, 24};
// it comes from built binaries:
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];
// using UDG from ASM
extern uint8_t horizontal[];
extern uint8_t vertical[];
extern uint8_t corner_left[];
extern uint8_t corner_right[];
extern uint8_t corner_bottom_right[];
extern uint8_t corner_bottom_left[];
// or using UDG from just code

uint8_t map[25][16] = {
{49,17,17,17,17,17,17,17,17,17,17,17,17,17,17,20},
{32,0,0,0,0,0,0,8,128,0,0,0,0,0,0,2},
{41,153,153,153,153,153,153,8,137,153,153,153,153,153,153,2},
{41,3,20,144,49,17,73,8,137,19,273,285,144,49,73,2},
{43,2,2,144,32,0,41,8,137,18,0,18,144,32,43,2},
{41,5,22,144,81,17,105,8,137,14,272,271,144,81,105,2},
{41,0,0,144,0,0,9,0,9,0,0,0,144,0,9,2},
{41,153,153,153,153,153,153,153,153,153,153,153,153,153,153,2},
{41,1,17,144,41,1,17,17,17,17,25,2,144,17,25,2},
{41,0,0,144,41,0,0,7,160,0,9,2,144,0,9,2},
{41,153,153,144,41,153,153,7,169,153,153,2,153,153,153,2},
{33,17,17,144,33,17,25,94,1529,1,17,18,144,17,17,18},
{32,0,0,144,32,0,9,0,9,0,0,2,144,0,0,2},
{41,153,153,144,41,153,153,153,153,153,153,2,153,153,153,2},
{33,17,17,144,41,3,17,31,241,17,73,2,144,17,17,18},
{32,0,0,144,41,2,0,15,240,0,41,2,144,0,0,2},
{41,153,153,144,41,2,0,15,240,0,41,2,153,153,153,2},
{41,3,20,144,41,5,17,17,17,17,105,2,144,49,73,2},
{41,2,2,144,9,0,0,0,0,0,9,0,144,32,41,2},
{43,2,2,153,153,153,153,153,153,153,153,153,144,32,43,2},
{41,5,22,144,17,17,144,17,17,144,17,17,144,81,105,2},
{41,0,0,144,0,0,144,0,0,144,0,0,144,0,9,2},
{41,153,153,153,153,153,153,153,153,153,153,153,153,153,153,2},
{81,17,17,17,17,17,17,17,17,17,17,17,17,17,17,22},
};

uint8_t correspondence[12] = {' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

extern uint8_t cartoon0[];

// globals are supposed to generate less code and with 128k of memory it's important
struct sprite {
    struct sp1_ss* sp;
    uint8_t x;
    uint8_t y;
    uint8_t offset;
};

uint8_t eating = 0;
struct sprite pacman;
JOYFUNC joy;
// redefine this array to allow define keys
udk_t joy_keys = { IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_p, IN_KEY_SCANCODE_o, IN_KEY_SCANCODE_a, IN_KEY_SCANCODE_q };
uint16_t in;
// reusable vars
uint8_t row;
uint8_t col;
uint8_t col2;
uint8_t current;


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
  c->attr      = INK_YELLOW;
}

struct sp1_ss * add_sprite() {
  struct sp1_ss * sp;
   sp = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColour);

  return sp;
}

void all_lives_lost() {
  uint16_t has_kempston = in_stick_kempston();

   while(1) {
      if(in_key_pressed( IN_KEY_SCANCODE_SPACE )) {
          joy = (JOYFUNC)in_stick_keyboard;
          break;
      } else if(has_kempston == 0 && (in_stick_kempston() & IN_STICK_FIRE)) {
          joy = (JOYFUNC)in_stick_kempston;
          break;
      }
  }

}

void check_keys()
{
    // checks keys
    // allow jump in directions
    if ((in & IN_STICK_UP) && pacman.y > 0) {
        --pacman.y;
    } else if((in & IN_STICK_DOWN) && pacman.y < 23) {
        ++pacman.y;
    } else if((in & IN_STICK_LEFT) && pacman.x > 0) {
        --pacman.x;
    } else if((in & IN_STICK_RIGHT) && pacman.x < 31) {
        ++pacman.x;
    }
}

int main()
{
  // show paging capabilities.
  show_intro();
  all_lives_lost();

  // now sp1
  pacman.sp = add_sprite();
  pacman.offset = 1;
  pacman.y = 10;
  pacman.x = 12;
  // painting an UDG is just assigning it to any char
  // row, col, char

  zx_border(INK_WHITE);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_BLACK,
                  ' ' );
  zx_border(INK_BLACK);

  sp1_Invalidate(&full_screen);

  sp1_TileEntry('a', horizontal);
  sp1_TileEntry('b', vertical);
  sp1_TileEntry('c', corner_left);
  sp1_TileEntry('d', corner_right);
  sp1_TileEntry('e', corner_bottom_left);
  sp1_TileEntry('f', corner_bottom_right);

  for(row = 0; row != 24; ++row) {
      col2 = 0;
      for(col = 0; col != 16; ++col) {
        current = map[row][col];
        sp1_PrintAt(row, col2, INK_BLUE | PAPER_BLACK, correspondence[(current & 0b11110000) >> 4 ]);
        sp1_PrintAt(row, col2 + 1, INK_BLUE | PAPER_BLACK, correspondence[current & 0b00001111]);
        col2 += 2;
      }

  }

  sp1_UpdateNow();

  while(1) {
     in = (joy)(&joy_keys);
     check_keys();
     // todo FSM checks
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
     sp1_UpdateNow();
  }
}

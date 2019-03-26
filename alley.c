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
#include "int.h"

#define RIGHTC1 1
#define RIGHTC2 33
#define LEFTC1 65
#define LEFTC2 97
#define UP1 129
#define UP2 161
#define DOWN1 193
#define DOWN2 225


#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4
#define NONE 250

// screen rectangle
struct sp1_Rect full_screen = {0, 0, 32, 24};
// it comes from built binaries:
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];
// red ghost
extern uint8_t red_ghost1[];
extern uint8_t red_ghost2[];
// using UDG from ASM
extern uint8_t horizontal[];
extern uint8_t vertical[];
extern uint8_t corner_left[];
extern uint8_t corner_right[];
extern uint8_t corner_bottom_right[];
extern uint8_t corner_bottom_left[];
extern uint8_t pill[];
extern uint8_t vertileft[];
extern uint8_t vertiright[];
extern uint8_t topvertileft[];
extern uint8_t topvertiright[];
extern uint8_t ghostpill[];

int8_t dx = 0;
int8_t dy = 0;
// or using UDG from just code
uint8_t map[25][32] = {
{3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,13,14,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4},
{2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,12,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
{2,9,9,9,9,9,9,9,9,9,9,9,9,9,0,8,12,9,9,9,9,9,9,9,9,9,9,9,9,9,0,2},
{2,9,0,3,1,4,9,0,3,1,1,1,4,9,0,8,12,9,0,3,1,1,1,4,9,0,3,1,4,9,0,2},
{2,11,0,2,0,2,9,0,2,0,0,0,2,9,0,8,12,9,0,2,0,0,0,2,9,0,2,0,2,11,0,2},
{2,9,0,5,1,6,9,0,5,1,1,1,6,9,0,8,12,9,0,5,1,1,1,6,9,0,5,1,6,9,0,2},
{2,9,0,0,0,0,9,0,0,0,0,0,0,9,0,0,0,9,0,0,0,0,0,0,9,0,0,0,0,9,0,2},
{2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,2},
{2,9,0,1,1,1,9,0,2,9,0,1,1,1,1,13,14,1,1,1,1,9,0,2,9,0,1,1,1,9,0,2},
{2,9,0,0,0,0,9,0,2,9,0,0,0,0,0,8,12,0,0,0,0,9,0,2,9,0,0,0,0,9,0,2},
{2,9,9,9,9,9,9,0,2,9,9,9,9,9,0,8,12,9,9,9,9,9,0,2,9,9,9,9,9,9,0,2},
{5,1,1,1,1,1,9,0,2,1,1,1,1,9,0,8,12,9,0,1,1,1,1,2,9,0,1,1,1,1,1,6},
{0,0,0,0,0,0,9,0,2,0,0,0,0,9,0,0,0,9,0,0,0,0,0,2,9,0,0,0,0,0,0,0},
{0,9,9,9,9,9,9,0,2,9,9,9,9,9,9,9,9,9,9,9,9,9,0,2,9,9,9,9,9,9,0,0},
{3,1,1,1,1,1,9,0,2,9,0,3,1,1,1,0,0,1,1,1,4,9,0,2,9,0,1,1,1,1,1,4},
{2,0,0,0,0,0,9,0,2,9,0,2,0,0,0,0,0,0,0,0,2,9,0,2,9,0,0,0,0,0,0,2},
{2,9,9,9,9,9,9,0,2,9,0,2,0,0,0,0,0,0,0,0,2,9,0,2,9,9,9,9,9,9,0,2},
{2,9,0,3,1,4,9,0,2,9,0,5,1,1,1,1,1,1,1,1,6,9,0,2,9,0,3,1,4,9,0,2},
{2,9,0,2,0,2,9,0,0,9,0,0,0,0,0,0,0,0,0,0,0,9,0,0,9,0,2,0,2,9,0,2},
{2,11,0,2,0,2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,2,0,2,11,0,2},
{2,9,0,5,1,6,9,0,1,1,1,1,9,0,1,1,1,1,9,0,1,1,1,1,9,0,5,1,6,9,0,2},
{2,9,0,0,0,0,9,0,0,0,0,0,9,0,0,0,0,0,9,0,0,0,0,0,9,0,0,0,0,9,0,2},
{2,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,0,2},
{5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6},
};


// 15
uint8_t correspondence[] = {' ',       'a',     'b',        'c',     'd',     'e',      'f',      'g',      'h',      'i',       'j',     'k',      'l',         'm',           'n'};
uint8_t colors[] =         {INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_BLUE, INK_WHITE, INK_BLUE, INK_MAGENTA, INK_BLUE,   INK_BLUE,     INK_BLUE};

extern uint8_t cartoon0[];

// globals are supposed to generate less code and with 128k of memory it's important
struct sprite {
    struct sp1_ss* sp;
    uint8_t x;
    uint8_t y;
    uint8_t offset;
    uint8_t currentoffset;
};

uint8_t eating = 0;
struct sprite pacman;
struct sprite ghost_red;
struct sprite ghost_cyan;
JOYFUNC joy;
// redefine this array to allow define keys
udk_t joy_keys = { IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_p, IN_KEY_SCANCODE_o, IN_KEY_SCANCODE_a, IN_KEY_SCANCODE_q };
uint16_t in;
// reusable vars
uint8_t row;
uint8_t col;
uint8_t current;

uint8_t frame = 0;


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

static void initialiseColourGhostRed(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;

    c->attr      = INK_RED;
}

static void initialiseColourGhostCyan(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;
    c->attr      = INK_CYAN;
}

struct sp1_ss * add_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_XOR1LB, SP1_TYPE_1BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_XOR1,    SP1_TYPE_1BYTE, (int)sprite_protar2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_XOR1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColour);

  return sp;
}

struct sp1_ss * add_ghost_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_XOR1LB, SP1_TYPE_1BYTE, 3, (int)red_ghost1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_XOR1,    SP1_TYPE_1BYTE, (int)red_ghost2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_XOR1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourGhostRed);

  return sp;
}

struct sp1_ss * add_ghost_cyan_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_XOR1LB, SP1_TYPE_1BYTE, 3, (int)red_ghost1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_XOR1,    SP1_TYPE_1BYTE, (int)red_ghost2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_XOR1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourGhostCyan);

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
    if ((in & IN_STICK_UP)) {
        dy = -1;
        pacman.currentoffset = UP1;

    } if((in & IN_STICK_DOWN)) {
        dy = 1;
        pacman.currentoffset = DOWN1;
    }

    if((in & IN_STICK_LEFT)) {
        dx = -1;
        pacman.currentoffset = LEFTC1;
    } else if((in & IN_STICK_RIGHT)) {
        dx = 1;
        pacman.currentoffset = RIGHTC1;
    }
}

uint8_t allow_next(uint8_t next) {
    return next == 9 || next == 16 || next == 11;
}


void check_fsm() {
    row = pacman.y + 1;
    if(row > 22) {
        row = 22;
    }
    col = pacman.x;
    if(col > 31) {
        col = 31;
    }

    current = map[row][col];

    if(allow_next(map[row + dy][col + dx])) {
        pacman.y += dy;
        pacman.x += dx;
    } else if (dy != 0) {
        dy = 0;
        dx = 0;
    }

    if(frame == 5) {
        pacman.offset = pacman.currentoffset + 32;
    } else if (frame == 0) {
        pacman.offset = pacman.currentoffset;
    }

}


int main()
{
  setup_int();
  // show paging capabilities.
  //show_intro();
  all_lives_lost();

  // now sp1
  pacman.sp = add_sprite();
  pacman.offset = 1;
  pacman.currentoffset = 1;

  pacman.y = 21;
  pacman.x = 14;

  ghost_red.sp = add_ghost_sprite();
  ghost_red.offset = 1;
  ghost_red.y = 21;
  ghost_red.x = 21;

  ghost_cyan.sp = add_ghost_cyan_sprite();
  ghost_cyan.offset = 1;
  ghost_cyan.y = 21;
  ghost_cyan.x = 25;

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
  sp1_TileEntry('i', pill);
  sp1_TileEntry('h', vertileft);
  sp1_TileEntry('k', ghostpill);
  sp1_TileEntry('l', vertiright);
  sp1_TileEntry('m', topvertileft);
  sp1_TileEntry('n', topvertiright);

  for(row = 0; row != 24; ++row) {
      for(col = 0; col != 32; ++col) {
        current = map[row][col];
        sp1_PrintAt(row, col, colors[current] | PAPER_BLACK, correspondence[current]);
      }

  }

  sp1_UpdateNow();

  while(1) {
     in = (joy)(&joy_keys);
     check_keys();
     check_fsm();
     // todo FSM checks
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
     sp1_MoveSprAbs(ghost_red.sp, &full_screen, (void*) ghost_red.offset, ghost_red.y, ghost_red.x, 0, 0);
     sp1_MoveSprAbs(ghost_cyan.sp, &full_screen, (void*) ghost_cyan.offset, ghost_cyan.y, ghost_cyan.x, 0, 0);
     wait();

     sp1_UpdateNow();

     frame += 1;

     if(frame == 10) {
        frame = 0;
     }
  }
}

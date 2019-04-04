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
#define DECIDED_DIRECTION 1
#define UNDECIDED 0

#define GHOST_RED 1
#define GHOST_RED2 33
#define GHOST_CYAN 65
#define GHOST_CYAN2 97
#define GHOST_YELLOW 129
#define GHOST_YELLOW2 161
#define GHOST_MAGENTA 193
#define GHOST_MAGENTA2 225

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

int8_t dx2 = 0;
int8_t dy2 = 0;

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
    uint8_t active;
    uint8_t target;
};

uint8_t pill_eaten = NONE;
struct sprite pacman;
struct sprite ghost_red;
struct sprite ghost_cyan;
struct sprite ghost_magenta;
struct sprite ghost_yellow;

struct sprite * ghosts[5] = {&ghost_red, &ghost_cyan, &ghost_magenta, &ghost_yellow, &ghost_magenta};

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

static void initialiseColourBlue(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_BLUE;
}

static void initialiseColourWhite(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_WHITE;
}

static void initialiseColourYellow(unsigned int count, struct sp1_cs *c)
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

static void initialiseColourGhostMagenta(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;
    c->attr      = INK_MAGENTA;
}

struct sp1_ss * add_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_XOR1LB, SP1_TYPE_1BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_XOR1,    SP1_TYPE_1BYTE, (int)sprite_protar2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_XOR1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourYellow);

  return sp;
}

struct sp1_ss * add_ghost_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)red_ghost1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)red_ghost2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  return sp;
}

struct sp1_ss * add_ghost_red_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();

  sp1_IterateSprChar(sp, initialiseColourGhostRed);

  return sp;
}

struct sp1_ss * add_ghost_cyan_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();

  sp1_IterateSprChar(sp, initialiseColourGhostCyan);

  return sp;
}

struct sp1_ss * add_ghost_magenta_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();

  sp1_IterateSprChar(sp, initialiseColourGhostMagenta);

  return sp;
}

struct sp1_ss * add_ghost_yellow_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();

  sp1_IterateSprChar(sp, initialiseColourYellow);

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

uint8_t allow_next(uint8_t next) {
    return next == 9 || next == 16 || next == 11;
}


void check_keys()
{
    // checks keys
    // allow jump in directions
    if ((in & IN_STICK_UP) && allow_next(map[row - 1][col])) {
        dy = -1;
        pacman.currentoffset = UP1;

    } else if((in & IN_STICK_DOWN) && allow_next(map[row + 1][col])) {
        dy = 1;
        pacman.currentoffset = DOWN1;
    }

    if((in & IN_STICK_LEFT) && allow_next(map[row][col - 1])) {
        dx = -1;
        pacman.currentoffset = LEFTC1;
    } else if((in & IN_STICK_RIGHT) && allow_next(map[row][col + 1])) {
        dx = 1;
        pacman.currentoffset = RIGHTC1;
    }
}

void iteratecolours(void * func) {
    sp1_IterateSprChar(ghost_red.sp, func);
    sp1_IterateSprChar(ghost_cyan.sp, func);
    sp1_IterateSprChar(ghost_magenta.sp, func);
    sp1_IterateSprChar(ghost_yellow.sp, func);
}


void cyan_eaten() {
    ghost_cyan.y = 15;
    ghost_cyan.x = 12;
    ghost_cyan.active = 0;
    ghost_cyan.target = UNDECIDED;
}

void red_eaten() {
    ghost_red.y = 15;
    ghost_red.x = 14;
    ghost_red.active = 0;
    ghost_red.target = UNDECIDED;
}

void magenta_eaten() {
    ghost_magenta.y = 15;
    ghost_magenta.x = 16;
    ghost_magenta.active = 0;
    ghost_magenta.target = UNDECIDED;
}

void yellow_eaten() {
    ghost_yellow.y = 15;
    ghost_yellow.x = 18;
    ghost_yellow.active = 0;
    ghost_yellow.target = UNDECIDED;
}

uint8_t goto_xy(struct sprite * for_who, uint8_t x, uint8_t y) {
    if(for_who->x != x) {
        if(for_who->x > x) {
            --for_who->x;
        } else if(for_who->x < x) {
            ++for_who->x;
        }
    } else {
        if(for_who->y > y) {
            --for_who->y;
        } else if(for_who->y < y) {
            ++for_who->y;
        } else {
            return 1;
        }
    }
    return 0;
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
    // row and col must be set at this point
    check_keys();

    current = map[row][col];
    if(current != 16) {
        map[row][col] = 16;
        sp1_PrintAtInv(row, col,  INK_BLACK, ' ');
    }

    if(current == 11) {
        pill_eaten = 125;
        iteratecolours(initialiseColourBlue);
    }

    if(allow_next(map[row + dy][col + dx])) {
        pacman.y += dy;
        pacman.x += dx;
    } else if (dy != 0) {
        dy = 0;
        dx = 0;
    }

    if((tick & 1) == 0) {
        pacman.offset = pacman.currentoffset + 32;
        ghosts[frame]->offset = ghosts[frame]->currentoffset + 32;
    } else {
        pacman.offset = pacman.currentoffset;
        ghosts[frame]->offset = ghosts[frame]->currentoffset;
    }

    if(pill_eaten != NONE) {
        --pill_eaten;
        // initialiseColourWhite, initialiseColourBlue
        if(pill_eaten < 40) {
            if((frame & 1) == 0) {
                iteratecolours(initialiseColourBlue);
            } else {
                iteratecolours(initialiseColourWhite);
            }
        }
    }

    if(pill_eaten == 0) {
        pill_eaten = NONE;
        sp1_IterateSprChar(ghost_red.sp, initialiseColourGhostRed);
        sp1_IterateSprChar(ghost_cyan.sp, initialiseColourGhostCyan);
        sp1_IterateSprChar(ghost_magenta.sp, initialiseColourGhostMagenta);
        sp1_IterateSprChar(ghost_yellow.sp, initialiseColourYellow);
    }

    if(ghosts[frame]->active == 0) {
        //ghosts[frame]->active = goto_xy(ghosts[frame], 15, 12);
    } else {

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

  ghost_red.sp = add_ghost_red_sprite();
  ghost_red.offset = GHOST_RED;
  ghost_red.currentoffset = GHOST_RED;
  red_eaten();

  ghost_cyan.sp = add_ghost_cyan_sprite();
  ghost_cyan.offset = GHOST_CYAN;
  ghost_cyan.currentoffset = GHOST_CYAN;
  cyan_eaten();

  ghost_magenta.sp = add_ghost_magenta_sprite();
  ghost_magenta.offset = GHOST_MAGENTA;
  ghost_magenta.currentoffset = GHOST_CYAN;
  magenta_eaten();

  ghost_yellow.sp = add_ghost_yellow_sprite();
  ghost_yellow.offset = GHOST_YELLOW;
  ghost_yellow.currentoffset = GHOST_CYAN;
  yellow_eaten();

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
        sp1_PrintAt(row, col, colors[current] | INK_BLACK, correspondence[current]);
      }

  }

  sp1_UpdateNow();

  while(1) {
     in = (joy)(&joy_keys);
     check_fsm();
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
     sp1_MoveSprAbs(ghost_red.sp, &full_screen, (void*) ghost_red.offset, ghost_red.y, ghost_red.x, 0, 0);
     sp1_MoveSprAbs(ghost_cyan.sp, &full_screen, (void*) ghost_cyan.offset, ghost_cyan.y, ghost_cyan.x, 0, 0);
     sp1_MoveSprAbs(ghost_magenta.sp, &full_screen, (void*) ghost_magenta.offset, ghost_magenta.y, ghost_magenta.x, 0, 0);
     sp1_MoveSprAbs(ghost_yellow.sp, &full_screen, (void*) ghost_yellow.offset, ghost_yellow.y, ghost_yellow.x, 0, 0);

     wait();

     sp1_UpdateNow();

     frame += 1;

     if(frame == 5) {
        frame = 0;
     }
  }
}

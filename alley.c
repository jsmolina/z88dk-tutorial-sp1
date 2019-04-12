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
#define JAILED 20
#define JAILED_EXITING 1
#define ACTIVE 0

#define AYCTRL 65533
#define AYDATA 49149

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

uint8_t random_value;

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
    int8_t dx;
    int8_t dy;
    uint8_t default_x;
    uint8_t default_y;
    void *default_color;
};

uint8_t pill_eaten = NONE;
struct sprite pacman;
struct sprite ghost_red;
struct sprite ghost_cyan;
struct sprite ghost_magenta;
struct sprite ghost_yellow;

struct sprite * ghosts[4] = {&ghost_red, &ghost_cyan, &ghost_magenta, &ghost_yellow};

JOYFUNC joy;
// redefine this array to allow define keys
udk_t joy_keys = { IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_p, IN_KEY_SCANCODE_o, IN_KEY_SCANCODE_a, IN_KEY_SCANCODE_q };
uint16_t in;
// reusable vars
uint8_t row;
uint8_t col;
uint8_t current;
uint16_t points = 0;

uint8_t frame = 0;

uint8_t lives = 0;
uint8_t repaint_lives = 1;
uint8_t idx;
struct sprite * collided_sprite;

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


  return sp;
}

struct sp1_ss * add_ghost_cyan_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();


  return sp;
}

struct sp1_ss * add_ghost_magenta_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();


  return sp;
}

struct sp1_ss * add_ghost_yellow_sprite() {
  struct sp1_ss * sp = add_ghost_sprite();


  return sp;
}



uint8_t allow_next(uint8_t next) {
    return next == 9 || next == 16 || next == 11 || next == 18;
}


void check_keys()
{
    // checks keys
    // allow jump in directions
    if ((in & IN_STICK_UP) && allow_next(map[row - 1][col])) {
        pacman.dy = -1;
        pacman.currentoffset = UP1;

    } else if((in & IN_STICK_DOWN) && allow_next(map[row + 1][col])) {
        pacman.dy = 1;
        pacman.currentoffset = DOWN1;
    }

    if((in & IN_STICK_LEFT) && allow_next(map[row][col - 1])) {
        pacman.dx = -1;
        pacman.currentoffset = LEFTC1;
    } else if((in & IN_STICK_RIGHT) && allow_next(map[row][col + 1])) {
        pacman.dx = 1;
        pacman.currentoffset = RIGHTC1;
    }
}

void iteratecolours(void * func) {
    sp1_IterateSprChar(ghost_red.sp, func);
    sp1_IterateSprChar(ghost_cyan.sp, func);
    sp1_IterateSprChar(ghost_magenta.sp, func);
    sp1_IterateSprChar(ghost_yellow.sp, func);
}

void set_eaten(struct sprite * for_who) {
    for_who->x = for_who->default_x;
    for_who->y = for_who->default_y;
    for_who->active = JAILED;
    for_who->dx = 0;
    for_who->dy = 0;
    sp1_IterateSprChar(for_who->sp, for_who->default_color);
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
            return ACTIVE;
        }
    }
    return JAILED_EXITING;
}

struct sprite * has_collision() {
    for(idx = 0; idx != 4; ++idx) {
        if(pacman.x == ghosts[idx]->x && pacman.y == ghosts[idx]->y) {
            // eat
            return ghosts[idx];
        }
    }
    return NULL;
}

uint8_t allow_next_ghost_pos(int8_t dy, int8_t dx) {
    row = ghosts[idx]->y + 1;
    return allow_next(map[row + dy][ghosts[idx]->x + dx]);
}

uint8_t could_go(uint8_t dir) {
    switch(dir) {
        case DIR_RIGHT:
            return allow_next(map[row][ghosts[idx]->x + 1]) && ghosts[idx]->dx != -1;
        break;
        case DIR_LEFT:
            return allow_next(map[row][ghosts[idx]->x - 1]) && ghosts[idx]->dx != 1;
        break;
        case DIR_DOWN:
            return allow_next(map[row + 1][ghosts[idx]->x]) && ghosts[idx]->dy != -1;
        break;
        case DIR_UP:
            return allow_next(map[row - 1][ghosts[idx]->x]) && ghosts[idx]->dy != 1;
        break;
    }
    return 0;
}

void remember_where_we_go(int8_t dy, int8_t dx) {
    ghosts[idx]->dx = dx;
    ghosts[idx]->dy = dy;
}

void then_go(uint8_t dir) {
    switch(dir) {
        case DIR_RIGHT:
            remember_where_we_go(0, 1);
            ghosts[idx]->x += 1;
            break;
        case DIR_LEFT:
            remember_where_we_go(0, -1);
            ghosts[idx]->x -= 1;
            break;
        case DIR_DOWN:
            remember_where_we_go(1, 0);
            ghosts[idx]->y += 1;
            break;
        case DIR_UP:
            remember_where_we_go(-1, 0);
            ghosts[idx]->y -= 1;
            break;
    }
}

void move_one_ghost(uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4) {
    if((random_value < t1) && could_go(DIR_RIGHT)) {
        then_go(DIR_RIGHT);
        return;
    } else if((random_value < t2) && could_go(DIR_LEFT)) {
        then_go(DIR_LEFT);
        return;
    }

    if((random_value < t3) && could_go(DIR_DOWN)) {
        then_go(DIR_DOWN);
        return;
    } else if((random_value < t4) && could_go(DIR_UP)) {
        then_go(DIR_UP);
        return;
    }
    if(pill_eaten != NONE) {
        if(pacman.x < ghosts[idx]->x && could_go(DIR_RIGHT)) {
            then_go(DIR_RIGHT);
        } else if(pacman.x > ghosts[idx]->x && could_go(DIR_LEFT)) {
            then_go(DIR_LEFT);
        } else if(pacman.y < ghosts[idx]->y && could_go(DIR_DOWN)) {
            then_go(DIR_DOWN);
        } else if(pacman.y > ghosts[idx]->y && could_go(DIR_UP)) {
            then_go(DIR_UP);
        } else if(allow_next(map[row + ghosts[idx]->dy][ghosts[idx]->x + ghosts[idx]->dx])) {
            ghosts[idx]->x += ghosts[idx]->dx;
            ghosts[idx]->y += ghosts[idx]->dy;
        }
    } else {
        if(pacman.x > ghosts[idx]->x && could_go(DIR_RIGHT)) {
            then_go(DIR_RIGHT);
        } else if(pacman.x < ghosts[idx]->x && could_go(DIR_LEFT)) {
            then_go(DIR_LEFT);
        } else if(pacman.y > ghosts[idx]->y && could_go(DIR_DOWN)) {
            then_go(DIR_DOWN);
        } else if(pacman.y < ghosts[idx]->y && could_go(DIR_UP)) {
            then_go(DIR_UP);
        } else if(allow_next(map[row + ghosts[idx]->dy][ghosts[idx]->x + ghosts[idx]->dx])) {
            ghosts[idx]->x += ghosts[idx]->dx;
            ghosts[idx]->y += ghosts[idx]->dy;
        }
    }
}

void move_ghosts() {
    // &ghost_red, &ghost_cyan, &ghost_magenta, &ghost_yellow
    row = ghosts[idx]->y + 1;

    switch(idx) {
        case 0: // Rojo: Intenta estár detrás de Pac-Man en modo "Acoso"
            move_one_ghost(40, 80, 40, 100);
            break;
        case 1:
            // hasta que Pac-Man captura al menos 30 pildoras
            move_one_ghost(90, 120, 100, 160);
            break;
        case 2:
            // su comportamiento  siempre es llegar hacia el punto donde Pac-Man
            move_one_ghost(10, 20, 10, 20);
            break;
        case 3:
            // "a su bola"
            move_one_ghost(125, 255, 125, 255);
    }

/*
    // se queda un poco de tiempo en la "Casa de los Fantasmas" en el primer nivel
    // hasta que Pac-Man captura al menos 30 pildoras
    idx = 1;
    random_value = rand();
    move_one_ghost(90, 120, 80, 100);

    // Pink: su comportamiento  siempre es llegar hacia el punto donde Pac-Man se está moviendo.
    idx = 2;
    random_value = rand();
    move_one_ghost(3, 5, 3, 6);

    //  podemos explicar mejor como "a su bola"
    idx = 3;
    random_value = rand();
    move_one_ghost(125, 250, 130, 250);*/

}

void check_fsm() {
    random_value = rand();
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

    // pill eat
    current = map[row][col];
    if(current != 16 && current != 18) {
        map[row][col] = current + 7; // 9 + 7 = 16, 11 + 7 = 18
        sp1_PrintAtInv(row, col,  INK_BLACK, ' ');

        if(current == 9) {
            points += 10; // ten points each
        } else if(current == 11) {
            points += 50;  // energizers - are worth 50 points each
            pill_eaten = 125;
            iteratecolours(initialiseColourBlue);
        }
    }



    // side change
    if(pacman.y == 12) {
        if(pacman.x < 2 && pacman.dx == -1) {
            pacman.x = 29;
        } else if(pacman.x > 28 && pacman.dx == 1) {
            pacman.x = 1;
        }
    }

    if(ghosts[frame]->y == 12) {
        if(ghosts[frame]->x < 2 && ghosts[frame]->dx == -1) {
            ghosts[frame]->x = 29;
        } else if(ghosts[frame]->x > 28 && ghosts[frame]->dx == 1) {
            ghosts[frame]->x = 1;
        }
    }


    if(allow_next(map[row + pacman.dy][col + pacman.dx])) {
        pacman.y += pacman.dy;
        pacman.x += pacman.dx;
    } else if (pacman.dy != 0) {
        pacman.dy = 0;
        pacman.dx = 0;
    }

    if((tick & 1) == 0) {
        pacman.offset = pacman.currentoffset + 32;
        ghosts[frame]->offset = ghosts[frame]->currentoffset + 32;
    } else {
        pacman.offset = pacman.currentoffset;
        ghosts[frame]->offset = ghosts[frame]->currentoffset;
    }

    // IA FOR GHOSTS
    for(idx = 0; idx != 4; ++idx) {
        if(ghosts[idx]->active == JAILED_EXITING) {
            if(points > 300 || (points > 10 && idx != 1)) {
                ghosts[idx]->active = goto_xy(ghosts[idx], 15, 12);
            }
        } else if(ghosts[idx]->active == ACTIVE) {
            move_ghosts();
        } else {
            --ghosts[idx]->active;
        }
    }
    // while has eaten pill
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

        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            // eat
            set_eaten(collided_sprite);
        }
    }

    if(pill_eaten == 0) {
        pill_eaten = NONE;
        sp1_IterateSprChar(ghost_red.sp, initialiseColourGhostRed);
        sp1_IterateSprChar(ghost_cyan.sp, initialiseColourGhostCyan);
        sp1_IterateSprChar(ghost_magenta.sp, initialiseColourGhostMagenta);
        sp1_IterateSprChar(ghost_yellow.sp, initialiseColourYellow);
    }

    if(pill_eaten == NONE) {
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            loose_a_live();
        }
    }
}

void all_lives_lost() {
  zx_border(INK_BLACK);
  sp1_Invalidate(&full_screen);
  lives = 5;
  points = 0;
  pacman.y = 21;
  pacman.x = 14;
  repaint_lives = 1;

  ghost_red.offset = GHOST_RED;
  ghost_red.currentoffset = GHOST_RED;
  ghost_red.default_color =  initialiseColourGhostRed;
  ghost_red.default_y = 15;
  ghost_red.default_x = 14;
  set_eaten(&ghost_red);

  ghost_cyan.offset = GHOST_CYAN;
  ghost_cyan.currentoffset = GHOST_CYAN;
  ghost_cyan.default_color =  initialiseColourGhostCyan;
  ghost_cyan.default_y = 15;
  ghost_cyan.default_x = 12;
  set_eaten(&ghost_cyan);

  ghost_magenta.offset = GHOST_MAGENTA;
  ghost_magenta.currentoffset = GHOST_CYAN;
  ghost_magenta.default_color =  initialiseColourGhostMagenta;
  ghost_magenta.default_y = 15;
  ghost_magenta.default_x = 16;
  set_eaten(&ghost_magenta);

  ghost_yellow.offset = GHOST_YELLOW;
  ghost_yellow.currentoffset = GHOST_YELLOW;
  ghost_yellow.default_color =  initialiseColourYellow;
  ghost_yellow.default_y = 15;
  ghost_yellow.default_x = 18;
  set_eaten(&ghost_yellow);


  for(row = 0; row != 24; ++row) {
      for(col = 0; col != 32; ++col) {
        if(map[row][col] == 18) {
             map[row][col] = 11;
        } else if(map[row][col] == 16) {
            map[row][col] = 9;
        }
        current = map[row][col];
        sp1_PrintAtInv(row, col, colors[current] | INK_BLACK, correspondence[current]);
      }

  }

  sp1_PrintAt(0, 2, INK_RED | PAPER_BLACK, 'L');
  sp1_PrintAt(0, 3, INK_RED | PAPER_BLACK, 'I');
  sp1_PrintAt(0, 4, INK_RED | PAPER_BLACK, 'V');
  sp1_PrintAt(0, 5, INK_RED | PAPER_BLACK, 'E');
  sp1_PrintAt(0, 6, INK_RED | PAPER_BLACK, 'S');

  for(idx = 0; idx != 4; ++idx) {
      sp1_MoveSprAbs(ghosts[idx]->sp, &full_screen, (void*) ghosts[idx]->offset, ghosts[idx]->y, ghosts[idx]->x, 0, 0);
  }

  sp1_UpdateNow();

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
  srand(tick);
}



void loose_a_live() {
    repaint_lives = 1;
    --lives;
    set_eaten(&ghost_magenta);
    set_eaten(&ghost_red);
    set_eaten(&ghost_cyan);
    set_eaten(&ghost_yellow);

    pacman.y = 21;
    pacman.x = 14;
    pacman.dx = 0;
    pacman.dy = 0;
}

int main()
{
  setup_int();
  // show paging capabilities.

  // now sp1
  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_BLACK,
                  ' ' );

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

  pacman.sp = add_sprite();
  pacman.offset = 1;
  pacman.currentoffset = 1;

  ghost_red.sp = add_ghost_red_sprite();
  ghost_cyan.sp = add_ghost_cyan_sprite();
  ghost_magenta.sp = add_ghost_magenta_sprite();
  ghost_yellow.sp = add_ghost_yellow_sprite();

  // painting an UDG is just assigning it to any char
  // row, col, char

  zx_border(INK_WHITE);

  while(1) {
     if(lives == 0) {
        all_lives_lost();
     }

     in = (joy)(&joy_keys);
     check_fsm();
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
     for(idx = 0; idx != 4; ++idx) {
          sp1_MoveSprAbs(ghosts[idx]->sp, &full_screen, (void*) ghosts[idx]->offset, ghosts[idx]->y, ghosts[idx]->x, 0, 0);
     }


     if(repaint_lives) {
        sp1_PrintAtInv(0, 8, INK_CYAN | PAPER_BLACK, 48 + lives);
        repaint_lives = 0;
     }

     sp1_UpdateNow();

     frame += 1;

     if(frame == 5) {
        frame = 0;
     }

     wait();
  }
}

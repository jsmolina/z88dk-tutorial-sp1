#ifndef GLOBALS_H
#define GLOBALS_H

#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>

#define MAP1_Y_SIDE_CHG 12
#define MAP2_Y_SIDE_CHG 10
#define MAP3_Y_SIDE_CHG 12
#define RIGHTC1 1
#define RIGHTC2 33
#define RIGHTC3 65
#define LEFTC1 97
#define LEFTC2 129
#define LEFTC3 161
#define UP1 193
#define UP2 225
#define UP3 257
#define DOWN1 289
#define DOWN2 321
#define DOWN3 353
#define NCLS 32

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
#define GHOST_FRIGHTENED 257
#define GHOST_FRIGHTENED2 289
#define GHOST_EYES 321
#define MAP1_TOTAL_POINTS 238
#define MAP2_TOTAL_POINTS 251
#define MAP3_TOTAL_POINTS 254

#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4
#define NONE 250
#define FRIGHTENED 25
#define JAILED 20
#define GETTING_JAILED 30
#define JAILED_EXITING 1
#define CHASE 0
#define SCATTER 40

// ghost index
#define GCYAN 0
#define GRED 1
#define GMAGENTA 2
#define GYELLOW 3

#define AYCTRL 65533
#define AYDATA 49149

#define Y_GHOSTS_HOME_MAP1 15;
#define Y_GHOSTS_HOME_MAP2  10;
#define Y_GHOSTS_HOME_MAP3  12;

// map integer correspondence
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define H 8
#define I 9
#define J 10    // esta bien un 10???
#define K 11
#define L 12
#define M 13
#define S 19
#define T 20
#define U 21
#define V 22
#define W 23
#define X 24
#define Y 25
// side change
#define s 250
#define t 251

#define CHERRY 1
#define STRAWBERRY 33
#define COCKTAIL 65
#define APPLE 97
#define PEAR 129

#define GAME_OVER 1
#define READY 25

#define POINTS_100 1
#define POINTS_200 25

// screen rectangle
extern struct sp1_Rect full_screen;

// globals are supposed to generate less code and with 128k of memory it's important

struct spritep {
    struct sp1_ss* sp;
    struct sp1_ss* alt;
    uint8_t x;
    uint8_t y;
    uint16_t offset;
    uint16_t currentoffset;
    uint8_t active;
    uint8_t direction;
    uint8_t default_x;
    uint8_t default_y;
    uint8_t last_dir;
    void *default_color;
};

struct bonus {
    struct sp1_ss* sp;
    uint8_t x;
    uint8_t y;
    uint8_t offset;
    uint8_t showing;
};

// it comes from built binaries:
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];

extern uint8_t sprite_protar_dead1[];
extern uint8_t sprite_protar_dead2[];

// points
extern uint8_t puntos1[];
extern uint8_t puntos2[];

// extra
extern uint8_t cerezas1[];
extern uint8_t cerezas2[];

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
extern uint8_t bottomvertileft[];

extern uint8_t terminal_left[];
extern uint8_t terminal_right[];
extern uint8_t terminal_up[];
extern uint8_t terminal_down[];

extern uint8_t union_left[];
extern uint8_t union_right[];
extern uint8_t door[];

extern uint8_t ghostpill[];
extern uint8_t cartoon0[];

extern uint8_t map[25][32];
extern uint8_t map2[25][32];
extern uint8_t map3[25][32];
extern uint8_t random_value;
extern uint8_t tmp_val;
extern uint8_t tmp_val2;
extern uint8_t exit_count;

extern uint8_t pill_eaten;
extern struct spritep pacman;
extern struct spritep ghost_red;
extern struct spritep ghost_cyan;
extern struct spritep ghost_magenta;
extern struct spritep ghost_yellow;
extern struct spritep * ghosts[4];
extern struct bonus cherry;
extern struct sp1_ss* billboard;
extern struct sp1_ss* points_sp;

extern JOYFUNC joy;
extern udk_t joy_keys;
extern uint16_t in;

extern uint8_t row;
extern uint8_t col;
extern uint8_t current;
extern uint16_t points;
extern uint8_t remaining_points;
extern uint8_t reached_level;
extern uint8_t slowticker;
extern uint8_t frame;
extern uint8_t showing_points;

extern uint8_t map_num;
extern uint8_t lives;
extern uint8_t level;
extern uint8_t repaint_lives;
extern uint8_t idx;
extern uint16_t matrixrow;
extern uint16_t matrixrow_ghost;
extern struct spritep * collided_sprite;

// billboard (game over / ready)
extern uint8_t letterboxes1[];
extern uint8_t letterboxes2[];
extern uint8_t letterboxes3[];
extern uint8_t letterboxes4[];
extern uint8_t letterboxes5[];
extern uint8_t letterboxes6[];
extern uint8_t letterboxes7[];
extern uint8_t letterboxes8[];
extern uint8_t letterboxes9[];

extern uint8_t vidas[];
extern uint8_t numbers[];
extern uint8_t abecedary[];
extern uint8_t * currentmap;

extern uint8_t aybank[83];
#endif


#ifndef GLOBALS_H
#define GLOBALS_H

#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>


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

#define DIR_UP 1
#define DIR_DOWN 2
#define DIR_LEFT 3
#define DIR_RIGHT 4
#define NONE 250
#define ELUDE 25
#define JAILED 20
#define JAILED_EXITING 1
#define ACTIVE 0

#define AYCTRL 65533
#define AYDATA 49149

// map integer correspondence
#define A 1
#define B 2
#define C 3
#define D 4
#define E 5
#define F 6
#define H 8
#define I 9
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

#define CHERRY 1
#define STRAWBERRY 33
#define COCKTAIL 65
#define APPLE 97
#define PEAR 129

// screen rectangle
extern struct sp1_Rect full_screen;

// globals are supposed to generate less code and with 128k of memory it's important
struct sprite {
    struct sp1_ss* sp;
    struct sp1_ss* alt;
    uint8_t x;
    uint8_t y;
    uint16_t offset;
    uint16_t currentoffset;
    uint8_t active;
    int8_t dx;
    int8_t dy;
    uint8_t default_x;
    uint8_t default_y;
    void *default_color;
};

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
extern uint8_t random_value;

extern uint8_t pill_eaten;
extern struct spritep pacman;
extern struct sprite ghost_red;
extern struct sprite ghost_cyan;
extern struct sprite ghost_magenta;
extern struct sprite ghost_yellow;
extern struct sprite * ghosts[4];
extern struct bonus cherry;

extern JOYFUNC joy;
extern udk_t joy_keys;
extern uint16_t in;

extern uint8_t row;
extern uint8_t col;
extern uint8_t current;
extern uint16_t points;
extern uint8_t remaining_points;
extern uint8_t speed;
extern uint8_t frame;

extern uint8_t lives;
extern uint8_t level;
extern uint8_t repaint_lives;
extern uint8_t idx;
extern struct sprite * collided_sprite;

#endif


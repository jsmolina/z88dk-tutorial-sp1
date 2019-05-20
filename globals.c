#include "globals.h"

struct sp1_Rect full_screen = {0, 0, 32, 24};

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




uint8_t pill_eaten = NONE;
struct sprite pacman;
struct sprite ghost_red;
struct sprite ghost_cyan;
struct sprite ghost_magenta;
struct sprite ghost_yellow;
struct bonus cherry;

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
uint8_t remaining_points;

uint8_t frame = 0;

uint8_t lives = 0;
uint8_t repaint_lives = 1;
uint8_t speed = 5;
uint8_t idx;
struct sprite * collided_sprite;

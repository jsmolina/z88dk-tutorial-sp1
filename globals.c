#include "globals.h"

struct sp1_Rect full_screen = {0, 0, 32, 24};

uint8_t random_value;

// or using UDG from just code
uint8_t map[25][32] = {
{C,A,A,A,A,A,A,A,A,A,A,A,A,A,A,M,M,A,A,A,A,A,A,A,A,A,A,A,A,A,A,D},
{B,0,0,0,0,0,0,0,0,0,0,0,0,0,0,B,B,0,0,0,0,0,0,0,0,0,0,0,0,0,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,C,A,D,I,0,C,A,A,A,D,I,0,B,B,I,0,C,A,A,A,D,I,0,C,A,D,I,0,B},
{B,K,0,B,0,B,I,0,B,0,0,0,B,I,0,B,B,I,0,B,0,0,0,B,I,0,B,0,B,K,0,B},
{B,I,0,E,A,F,I,0,E,A,A,A,F,I,0,E,F,I,0,E,A,A,A,F,I,0,E,A,F,I,0,B},
{B,I,0,0,0,0,I,0,0,0,0,0,0,I,0,0,0,I,0,0,0,0,0,0,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,S,A,T,I,0,U,I,0,S,A,A,A,M,M,A,A,A,T,I,0,U,I,0,S,A,T,I,0,B},
{B,I,0,0,0,0,I,0,B,I,0,0,0,0,0,B,B,0,0,0,0,I,0,B,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,0,B,I,I,I,I,I,0,B,B,I,I,I,I,I,0,B,I,I,I,I,I,I,0,B},
{E,A,A,A,A,T,I,0,X,A,A,A,T,I,0,E,F,I,0,S,A,A,A,W,I,0,S,A,A,A,A,F},
{0,0,0,0,0,0,I,0,B,0,0,0,0,I,0,0,0,I,0,0,0,0,0,B,I,0,0,0,0,0,0,0},
{0,I,I,I,I,I,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,I,I,I,I,I,0,0},
{C,A,A,A,A,T,I,0,B,I,0,C,A,A,A,Y,Y,A,A,A,D,I,0,B,I,0,S,A,A,A,A,D},
{B,0,0,0,0,0,I,0,B,I,0,B,0,0,0,0,0,0,0,0,B,I,0,B,I,0,0,0,0,0,0,B},
{B,I,I,I,I,I,I,0,B,I,0,B,0,0,0,0,0,0,0,0,B,I,0,B,I,I,I,I,I,I,0,B},
{B,I,0,C,A,D,I,0,V,I,0,E,A,A,A,A,A,A,A,A,F,I,0,V,I,0,C,A,D,I,0,B},
{B,I,0,B,0,B,I,0,0,I,0,0,0,0,0,0,0,0,0,0,0,I,0,0,I,0,B,0,B,I,0,B},
{B,K,0,B,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,0,B,K,0,B},
{B,I,0,E,A,F,I,0,S,A,A,T,I,0,S,A,A,T,I,0,S,A,A,T,I,0,E,A,F,I,0,B},
{B,I,0,0,0,0,I,0,0,0,0,0,I,0,0,0,0,0,I,0,0,0,0,0,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{E,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,F},
};


// 15




uint8_t pill_eaten = NONE;
struct spritep pacman;
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
uint8_t speed;
uint8_t idx;
struct sprite * collided_sprite;

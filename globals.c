#include "globals.h"

struct sp1_Rect full_screen = {0, 0, 32, 24};

uint8_t random_value;
uint8_t tmp_val;
uint8_t tmp_val2;
uint8_t exit_count;

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
{C,A,A,A,A,T,I,0,B,I,0,C,A,A,T,Y,Y,S,A,A,D,I,0,B,I,0,S,A,A,A,A,D},
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

uint8_t map2[25][32] = {
{C,A,A,A,A,A,A,A,A,A,A,A,A,A,A,M,M,A,A,A,A,A,A,A,A,A,A,A,A,A,A,D},
{B,0,0,0,0,0,0,0,0,0,0,0,0,0,0,B,B,0,0,0,0,0,0,0,0,0,0,0,0,0,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,S,A,T,I,0,S,A,A,A,T,I,0,E,F,I,0,S,A,A,A,T,I,0,S,A,T,I,0,B},
{B,I,0,0,0,0,I,0,0,0,0,0,0,I,0,0,0,I,0,0,0,0,0,0,I,0,0,0,0,I,0,B},
{B,K,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,K,0,B},
{B,I,0,S,A,D,I,0,C,A,T,I,0,S,A,A,A,A,T,I,0,S,A,D,I,0,C,A,T,I,0,B},
{B,I,0,0,0,B,I,0,B,0,0,I,0,0,0,0,0,0,0,I,0,0,0,B,I,0,B,0,0,I,0,B},
{B,I,I,I,0,B,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,0,B,I,I,I,0,B},
{E,A,T,I,0,V,I,0,V,I,0,C,A,A,T,Y,Y,S,A,A,D,I,0,V,I,0,V,I,0,S,A,F},
{0,0,0,I,0,0,I,0,0,I,0,B,0,0,0,0,0,0,0,0,B,I,0,0,I,0,0,I,0,0,0,0},
{0,I,I,I,I,I,I,I,I,I,0,B,0,0,0,0,0,0,0,0,B,I,I,I,I,I,I,I,I,I,I,0},
{C,A,T,I,0,S,A,A,D,I,0,E,A,A,A,A,A,A,A,A,F,I,0,C,A,A,T,I,0,S,A,D},
{B,0,0,I,0,0,0,0,B,I,0,0,0,0,0,0,0,0,0,0,0,I,0,B,0,0,0,I,0,0,0,B},
{B,I,I,I,I,I,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,I,I,I,I,I,0,B},
{B,I,0,S,A,T,I,0,E,A,A,A,T,I,0,C,D,I,0,S,A,A,A,F,I,0,S,A,T,I,0,B},
{B,I,0,0,0,0,I,0,0,0,0,0,0,I,0,B,B,I,0,0,0,0,0,0,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,E,F,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,C,A,D,I,0,C,A,A,A,D,I,0,0,0,I,0,C,A,A,A,D,I,0,C,A,D,I,0,B},
{B,I,0,B,0,B,I,0,B,0,0,0,B,I,I,I,I,I,0,B,0,0,0,B,I,0,B,0,B,I,0,B},
{B,K,0,E,A,F,I,0,E,A,A,A,F,I,0,C,D,I,0,E,A,A,A,F,I,0,E,A,F,K,0,B},
{B,I,0,0,0,0,I,0,0,0,0,0,0,I,0,B,B,I,0,0,0,0,0,0,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{E,A,A,A,A,A,A,A,A,A,A,A,A,A,A,J,J,A,A,A,A,A,A,A,A,A,A,A,A,A,A,F}
};

uint8_t map3[25][32] = {
{C,A,A,A,A,A,A,A,A,A,A,M,M,A,A,A,A,A,A,M,M,A,A,A,A,A,A,A,A,A,A,D},
{B,0,0,0,0,0,0,0,0,0,0,B,B,0,0,0,0,0,0,B,B,0,0,0,0,0,0,0,0,0,0,B},
{B,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,U,I,0,C,A,D,I,0,B,B,I,0,S,T,I,0,B,B,I,0,C,A,D,I,0,U,I,0,B},
{B,K,0,B,I,0,B,0,B,I,0,B,B,I,0,0,0,I,0,B,B,I,0,B,0,B,I,0,B,K,0,B},
{B,I,0,V,I,0,E,A,F,I,0,E,F,I,I,I,I,I,0,E,F,I,0,E,A,F,I,0,V,I,0,B},
{B,I,0,0,I,0,0,0,0,I,0,0,0,I,0,C,D,I,0,0,0,I,0,0,0,0,I,0,0,I,0,B},
{B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B},
{B,I,0,S,A,T,I,0,C,A,T,I,0,S,A,J,J,A,T,I,0,S,A,D,I,0,S,A,T,I,0,B},
{B,I,0,0,0,0,I,0,B,0,0,I,0,0,0,0,0,0,0,I,0,0,0,B,I,0,0,0,0,I,0,B},
{B,I,I,I,I,I,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,I,I,I,I,I,0,B},
{E,A,A,T,I,0,S,A,F,I,0,C,A,A,T,Y,Y,S,A,A,D,I,0,E,A,T,I,0,S,A,A,F},
{0,0,0,0,I,0,0,0,0,I,0,B,0,0,0,0,0,0,0,0,B,I,0,0,0,0,I,0,0,0,0,0},
{0,I,I,I,I,I,I,I,I,I,0,B,0,0,0,0,0,0,0,0,B,I,I,I,I,I,I,I,I,I,I,0},
{C,A,A,T,I,0,S,A,D,I,0,E,A,A,A,A,A,A,A,A,F,I,0,C,A,T,I,0,S,A,A,D},
{B,0,0,0,I,0,0,0,B,I,0,0,0,0,0,0,0,0,0,0,0,I,0,B,0,0,I,0,0,0,0,B},
{B,I,I,I,I,I,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,I,I,I,I,I,0,B},
{B,I,0,C,A,T,I,0,E,A,T,I,0,S,T,I,0,S,T,I,0,S,A,F,I,0,S,A,D,I,0,B},
{B,I,0,B,0,0,I,0,0,0,0,I,0,0,0,I,0,0,0,I,0,0,0,0,I,0,0,0,B,I,0,B},
{B,I,0,B,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,I,0,B},
{B,K,0,V,I,0,C,D,I,0,S,A,T,I,0,S,T,I,0,S,A,T,I,0,C,D,I,0,V,K,0,B},
{B,I,0,0,I,0,B,B,I,0,0,0,0,I,0,0,0,I,0,0,0,0,I,0,B,B,I,0,0,I,0,B},
{B,I,I,I,I,0,B,B,I,I,I,I,I,I,I,I,I,I,I,I,I,I,I,0,B,B,I,I,I,I,0,B},
{E,A,A,A,A,A,J,J,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,A,J,J,A,A,A,A,A,F}
};

uint8_t * currentmap;
// todo puntero al mapa actual, posiciones diferentes para el mapa

// 15




uint8_t pill_eaten = NONE;
struct spritep pacman;
struct spritep ghost_red;
struct spritep ghost_cyan;
struct spritep ghost_magenta;
struct spritep ghost_yellow;
struct bonus cherry;
struct sp1_ss* billboard;
struct sp1_ss* points_sp;

struct spritep * ghosts[4] = {&ghost_cyan, &ghost_red, &ghost_magenta, &ghost_yellow};

JOYFUNC joy;
// redefine this array to allow define keys
udk_t joy_keys = { IN_KEY_SCANCODE_SPACE, IN_KEY_SCANCODE_p, IN_KEY_SCANCODE_o, IN_KEY_SCANCODE_a, IN_KEY_SCANCODE_q };
uint16_t in;
// reusable vars
uint8_t row;
uint8_t col;
uint8_t current;
uint16_t points = 0;
uint8_t remaining_points = MAP1_TOTAL_POINTS;

uint8_t frame = 0;

uint8_t lives = 0;
uint8_t level = 0;
uint8_t repaint_lives = 1;
uint8_t reached_level = 0;
uint8_t slowticker = 0;
uint8_t showing_points = NONE;
uint8_t idx;
uint16_t matrixrow;
uint16_t matrixrow_ghost;
uint8_t map_num = 1;
struct spritep * collided_sprite;

uint8_t aybank[83]; // 83 bytes bank

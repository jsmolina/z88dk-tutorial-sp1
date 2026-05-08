#include "globals.h"

struct sp1_Rect full_screen = {0, 0, 32, 24};

uint8_t random_value;
uint8_t tmp_val;
uint8_t tmp_val2;
uint8_t exit_count;

// Working map buffer - loaded from banked RAM on level load
uint8_t currentmap[25][32];

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
uint8_t remaining_points;

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

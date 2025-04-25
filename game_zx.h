#ifndef GAME_ZX
#define GAME_ZX

#include <z80.h>
#include <arch/zx.h>

void print_points();
void make_sound();
void printatstr(uint8_t y, uint8_t x, uint8_t attr, char *chars);
#endif

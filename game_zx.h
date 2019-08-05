#ifndef GAME_ZX
#define GAME_ZX

#include <z80.h>
#include <arch/zx.h>

extern void print_points();
extern void make_sound();
extern void printatstr(uint8_t y, uint8_t x, uint8_t attr, char *chars);
#endif

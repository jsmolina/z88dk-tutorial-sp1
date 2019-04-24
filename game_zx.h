#ifndef GAME_ZX
#define GAME_ZX

#include <z80.h>
#include <arch/zx.h>

#define zx_print_ink(a)     (screen_ink = (a))


extern void            zx_print_chr(unsigned char ui_row, unsigned char ui_col, unsigned char val) __z88dk_callee;
extern void            zx_print_int(unsigned char ui_row, unsigned char ui_col, unsigned int val) __z88dk_callee;
extern void            zx_print_str(unsigned char ui_row, unsigned char ui_col, unsigned char *s) __z88dk_callee;

#endif

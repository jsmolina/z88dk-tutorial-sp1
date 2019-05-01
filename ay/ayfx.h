#ifndef GAME_AY_H
#define GAME_AY_H

// Bit Flags Returned from ay_is_playing()

#define AY_PLAYING_NONE          0
#define AY_PLAYING_SONG          1
#define AY_PLAYING_FX            2

extern unsigned char ay_is_playing(void) __preserves_regs(d,e);

// AY Effects Player

extern void mfx_playe() __preserves_regs(a,b,c) __z88dk_callee;
extern void FXINIT() __preserves_regs(a,b,c) __z88dk_callee;


#endif

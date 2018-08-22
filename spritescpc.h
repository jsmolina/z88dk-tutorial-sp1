// sprites.h

extern unsigned char sprite_negro [];
extern unsigned char sprite_cubo [];

#asm
	._sprite_negro
	defb 0,0,0,0,0,0,0,0

	._sprite_cubo
	defb 47,47,47,47,47,47,47,47
#endasm
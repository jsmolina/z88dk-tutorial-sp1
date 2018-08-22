// sprites.h

extern unsigned char sprite_negro [];
extern unsigned char sprite_cubo [];
extern unsigned char sprite_moneda [];
extern unsigned char sprite_prota1 [];
extern unsigned char sprite_prota2 [];
extern unsigned char sprite_malo1 [];
extern unsigned char sprite_malo2 [];
extern unsigned char sprite_arbol [];
extern unsigned char sprite_casa [];

extern unsigned char fondov_x16 [];
extern unsigned char fondoh_x16 [];

#asm
	._sprite_negro
	defb 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	defb 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	defb 0,0,0,0

	._sprite_cubo
	defb 255,255,128,1,128,1,128,1,128,1,128,1,128,1,128,1
	defb 128,1,128,1,128,1,128,1,128,1,128,1,128,1,255,255
	defb 12,13,14,15

	._sprite_moneda
	defb 0,0,3,192,12,48,19,200
	defb 47,244,44,52,91,218,80,122
	defb 80,122,91,218,44,52,47,244
	defb 19,200,12,48,3,192,0,0
	defb 71,70,70,6

	._sprite_prota1
	defb 0,0,28,28,62,62,127,127
	defb 115,103,115,103,62,62,28,28
	defb 3,224,31,252,124,14,112,6
	defb 24,6,120,4,0,12,0,15
	defb 71,71,67,3

	._sprite_prota2
	defb 0,0,56,56,124,124,254,254
	defb 230,206,230,206,124,124,56,56
	defb 7,192,63,248,112,62,96,14
	defb 96,24,32,30,48,0,240,0
	defb 71,71,67,3

	._sprite_malo1
	defb 0,0,28,28,62,62,103,115
	defb 103,115,115,103,62,62,28,28
	defb 3,224,30,188,63,254,48,62
	defb 28,120,15,238,12,0,56,0
	defb 71,71,66,2

	._sprite_malo2
	defb 0,0,28,28,62,62,103,115
	defb 103,115,115,103,62,62,28,28
	defb 3,224,30,188,63,254,62,6
	defb 15,28,59,248,0,24,0,14
	defb 71,71,66,2

	._sprite_arbol
	defb 7,255,192,31,255,176,63,255
	defb 232,127,255,244,127,255,252,255
	defb 255,254,255,255,250,255,255,244
	defb 255,255,250,255,255,244,255,255
	defb 250,127,255,252,127,255,212,63
	defb 254,232, 27,213,176,6,170,192
	defb 3,137,128,1,219,0,0,126
	defb 0,0,62,0,0,28,0,0
	defb 28,0,0,60,0,0,126,0
	defb 68,68,4,68,68,4,66,66
	defb 66

	._sprite_casa
	defb 0,0,0,0,64,14,0,0
	defb 104,31,128,0,235,31,204,0
	defb 62,7,158,0,62,0,7,0
	defb 28,0,0,252,8,0,0,120
	defb 255,255,255,255,251,239,190,251
	defb 251,239,190,251,113,199,28,113
	defb 4,16,65,4,223,125,247,223
	defb 223,125,247,223,142,56,227,142
	defb 0,0,0,0,63,254,0,0
	defb 36,146,3,128,36,146,15,224
	defb 63,254,29,112,36,146,49,24
	defb 36,146,63,248,63,254,97,12
	defb 0,0,97,12,63,254,127,252
	defb 63,254,127,252,31,252,103,252
	defb 0,0,103,252,0,0,127,252
	defb 0,0,127,252,0,0,127,252
	defb 70,7,7,71,122,122,122,122
	defb 121,121,123,123,124,124,123,123

	._fondov_x16
	defb 0,0,0,0,0,0,0,0
	defb 0,0,0,0,0,0,0,0
	defb 0,0

	._fondoh_x16
	defb 0,0,0,0,0,0,0,0
	defb 0,0,0,0,0,0,0,0
	defb 0,0
#endasm
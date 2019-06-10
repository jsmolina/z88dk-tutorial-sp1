#ifndef SPRITES_H
#define SPRITES_H

#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include "globals.h"


extern struct sp1_ss * add_sprite();
extern struct sp1_ss * add_dead_prota_sprite();
extern struct sp1_ss * add_ghost_sprite();
extern struct sp1_ss * add_ghost_red_sprite();
extern struct sp1_ss * add_ghost_cyan_sprite();
extern struct sp1_ss * add_ghost_magenta_sprite();
extern struct sp1_ss * add_ghost_yellow_sprite();
extern struct sp1_ss * add_cherry_sprite();
extern struct sp1_ss * add_billboard_sprite();

extern void initialiseColourBlue(unsigned int count, struct sp1_cs *c);
extern void initialiseColourWhite(unsigned int count, struct sp1_cs *c);
extern void initialiseColourYellow(unsigned int count, struct sp1_cs *c);
extern void initialiseColourGhostCyan(unsigned int count, struct sp1_cs *c);
extern void initialiseColourGhostMagenta(unsigned int count, struct sp1_cs *c);
extern void initialiseColourGhostRed(unsigned int count, struct sp1_cs *c);
extern void initialiseColourGreenRed(unsigned int count, struct sp1_cs *c);
extern void initialiseColourRedYellowWhiteCyan(unsigned int count, struct sp1_cs *c);
extern void initialiseColourGreen(unsigned int count, struct sp1_cs *c);

#endif

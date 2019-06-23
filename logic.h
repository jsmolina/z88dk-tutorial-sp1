#ifndef LOGIC_H
#define LOGIC_H

#include "sprites.h"
#include "globals.h"
#include "int.h"


#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern void check_fsm();
extern uint8_t allow_next(uint8_t next);
extern void set_eaten(struct spritep * for_who);
extern void init_ghost(struct spritep * for_who);
extern void hide_cherry();
extern void show_cherry();
extern void nampac_go_home();
extern void all_ghosts_go_home();
extern void reset_map();
extern void set_ghosts_offsets();
extern void show_billboard(uint8_t offset);
extern void hide_billboard();
extern void paint_lives() ;
#endif

#ifndef LOGIC_H
#define LOGIC_H

#include "sprites.h"
#include "globals.h"
#include "int.h"


#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

void check_fsm(void);
uint8_t allow_next(uint8_t next);
void set_eaten(struct spritep * for_who);
void init_ghost(struct spritep * for_who);
void hide_cherry(void);
void show_cherry(void);
void nampac_go_home(void);
void all_ghosts_go_home(void);
void reset_map(void);
void reset_colors(struct spritep * for_who);
void set_ghosts_offsets(void);
void show_billboard(uint8_t offset);
void hide_billboard(void);
void paint_lives(void) ;
void hide_points(void);
void set_ghosts_default_coords(void);
void animation(void);

#endif

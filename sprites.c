#include "sprites.h"

void initialiseColourBlue(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_BLUE | BRIGHT;
}

void initialiseColourWhite(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_WHITE;
}

void initialiseColourWhiteBright(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_WHITE | BRIGHT;
}

void initialiseColourYellow(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_YELLOW | BRIGHT;
}

void initialiseColourGhostRed(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;

    c->attr      = INK_RED | BRIGHT;
}

void initialiseColourGhostCyan(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;
    c->attr      = INK_CYAN | BRIGHT;
}

void initialiseColourGhostMagenta(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
  (void)count;    /* Suppress compiler warning about unused parameter */

    c->attr_mask = SP1_AMASK_INK;
    c->attr      = INK_MAGENTA | BRIGHT;
}

void initialiseColourRedApple(unsigned int count, struct sp1_cs *c)
{
  // count defines each 8x8 block in the sprite, and thus it is possible to have a
  // multicolor sprite in 8x8 blocks
    if(count == 1 || count == 4) {
        c->attr      = INK_RED;
    } else {
        c->attr      = INK_RED | BRIGHT;
    }

    c->attr_mask = SP1_AMASK_INK;
}

void initialiseColourGreenRed(unsigned int count, struct sp1_cs *c)
{
  c->attr_mask = SP1_AMASK_INK;
  if(count < 2) {
     c->attr      = INK_GREEN;
  } else {
     c->attr      = INK_RED;
  }

  if (count != 1 && count != 4) {
     c->attr     |= BRIGHT;
  }
}

void initialiseColourGreen(unsigned int count, struct sp1_cs *c)
{
    c->attr_mask = SP1_AMASK_INK;
    if(count == 1 || count == 4) {
        c->attr      = INK_GREEN;
    } else {
        c->attr      = INK_GREEN | BRIGHT;
    }

}

void initialiseColourRedYellowWhiteCyan(unsigned int count, struct sp1_cs *c)
{
  c->attr_mask = SP1_AMASK_INK;
  if(count == 0) {
     c->attr      = INK_RED;
  } else if (count == 1){
     c->attr      = INK_YELLOW;
  } else if(count == 3) {
    c->attr = INK_WHITE;
  } else {
    c->attr = INK_CYAN;
  }
}


struct sp1_ss * add_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)sprite_protar2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourYellow);

  return sp;
}

struct sp1_ss * add_dead_prota_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_XOR1LB, SP1_TYPE_1BYTE, 3, (int)sprite_protar_dead1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_XOR1,    SP1_TYPE_1BYTE, (int)sprite_protar_dead2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_XOR1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourYellow);

  return sp;
}

struct sp1_ss * add_ghost_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)red_ghost1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)red_ghost2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  return sp;
}

struct sp1_ss * add_cherry_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)cerezas1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)cerezas2, 1);

  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourGreenRed);


  return sp;
}

struct sp1_ss * add_billboard_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 2, (int)letterboxes1, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes2, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes3, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes4, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes5, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes6, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes7, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes8, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)letterboxes9, 2);

  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  return sp;
}

struct sp1_ss * add_points_sprite() {
  struct sp1_ss * sp;
  sp = sp1_CreateSpr(SP1_DRAW_LOAD1LB, SP1_TYPE_1BYTE, 3, (int)puntos1, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1,    SP1_TYPE_1BYTE, (int)puntos2, 2);
  sp1_AddColSpr(sp, SP1_DRAW_LOAD1RB,  SP1_TYPE_1BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColourWhiteBright);

  return sp;
}

#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <input.h>
#include <sound.h> // for bit_beepfx()
#include <string.h>
#include <stdlib.h>
#include <intrinsic.h>
#include <input.h>
#include "logic.h"
#include "sprites.h"
#include "globals.h"
#include "int.h"
#include "game_zx.h"


void all_lives_lost() {
  zx_border(INK_BLACK);
  sp1_Invalidate(&full_screen);
  lives = 5;
  points = 0;
  pacman.y = 21;
  pacman.x = 14;
  repaint_lives = 1;

  ghost_red.offset = GHOST_RED;
  ghost_red.currentoffset = GHOST_RED;
  ghost_red.default_color =  initialiseColourGhostRed;
  ghost_red.default_y = 15;
  ghost_red.default_x = 14;
  set_eaten(&ghost_red);

  ghost_cyan.offset = GHOST_CYAN;
  ghost_cyan.currentoffset = GHOST_CYAN;
  ghost_cyan.default_color =  initialiseColourGhostCyan;
  ghost_cyan.default_y = 15;
  ghost_cyan.default_x = 12;
  set_eaten(&ghost_cyan);

  ghost_magenta.offset = GHOST_MAGENTA;
  ghost_magenta.currentoffset = GHOST_CYAN;
  ghost_magenta.default_color =  initialiseColourGhostMagenta;
  ghost_magenta.default_y = 15;
  ghost_magenta.default_x = 16;
  set_eaten(&ghost_magenta);

  ghost_yellow.offset = GHOST_YELLOW;
  ghost_yellow.currentoffset = GHOST_YELLOW;
  ghost_yellow.default_color =  initialiseColourYellow;
  ghost_yellow.default_y = 15;
  ghost_yellow.default_x = 18;
  set_eaten(&ghost_yellow);


  for(row = 0; row != 24; ++row) {
      for(col = 0; col != 32; ++col) {
        if(map[row][col] == 18) {
             map[row][col] = 11;
        } else if(map[row][col] == 16) {
            map[row][col] = 9;
        }
        current = map[row][col];
        sp1_PrintAtInv(row, col, colors[current] | INK_BLACK, correspondence[current]);
      }

  }

  sp1_PrintAtInv(0, 19, INK_RED | PAPER_BLACK, 'P');
  sp1_PrintAtInv(0, 20, INK_RED | PAPER_BLACK, 'O');
  sp1_PrintAtInv(0, 21, INK_RED | PAPER_BLACK, 'I');
  sp1_PrintAtInv(0, 22, INK_RED | PAPER_BLACK, 'N');
  sp1_PrintAtInv(0, 23, INK_RED | PAPER_BLACK, 'T');
  sp1_PrintAtInv(0, 24, INK_RED | PAPER_BLACK, 'S');

  sp1_PrintAt(0, 2, INK_RED | PAPER_BLACK, 'L');
  sp1_PrintAt(0, 3, INK_RED | PAPER_BLACK, 'I');
  sp1_PrintAt(0, 4, INK_RED | PAPER_BLACK, 'V');
  sp1_PrintAt(0, 5, INK_RED | PAPER_BLACK, 'E');
  sp1_PrintAt(0, 6, INK_RED | PAPER_BLACK, 'S');

  for(idx = 0; idx != 4; ++idx) {
      sp1_MoveSprAbs(ghosts[idx]->sp, &full_screen, (void*) ghosts[idx]->offset, ghosts[idx]->y, ghosts[idx]->x, 0, 0);
  }

  sp1_UpdateNow();

  uint16_t has_kempston = in_stick_kempston();

   while(1) {
      if(in_key_pressed( IN_KEY_SCANCODE_SPACE )) {
          joy = (JOYFUNC)in_stick_keyboard;
          break;
      } else if(has_kempston == 0 && (in_stick_kempston() & IN_STICK_FIRE)) {
          joy = (JOYFUNC)in_stick_kempston;
          break;
      }
  }
  srand(tick);
  pick = 1;

}


int main()
{
  setup_int();

  // now sp1
  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_BLACK,
                  ' ' );

  sp1_TileEntry('a', horizontal);
  sp1_TileEntry('b', vertical);
  sp1_TileEntry('c', corner_left);
  sp1_TileEntry('d', corner_right);
  sp1_TileEntry('e', corner_bottom_left);
  sp1_TileEntry('f', corner_bottom_right);
  sp1_TileEntry('i', pill);
  sp1_TileEntry('h', vertileft);
  sp1_TileEntry('k', ghostpill);
  sp1_TileEntry('l', vertiright);
  sp1_TileEntry('m', topvertileft);
  sp1_TileEntry('n', topvertiright);

  pacman.sp = add_sprite();
  pacman.alt = add_dead_prota_sprite();
  pacman.offset = 1;
  pacman.currentoffset = 1;

  ghost_red.sp = add_ghost_red_sprite();
  ghost_cyan.sp = add_ghost_cyan_sprite();
  ghost_magenta.sp = add_ghost_magenta_sprite();
  ghost_yellow.sp = add_ghost_yellow_sprite();

  // painting an UDG is just assigning it to any char
  // row, col, char

  zx_border(INK_WHITE);

  while(1) {

     if(lives == 0) {
        all_lives_lost();
     }

     in = (joy)(&joy_keys);
     check_fsm();
     // sprite, rectangle, offset (animations), y, x, rotationy, rotationx
     sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
     for(idx = 0; idx != 4; ++idx) {
          sp1_MoveSprAbs(ghosts[idx]->sp, &full_screen, (void*) ghosts[idx]->offset, ghosts[idx]->y, ghosts[idx]->x, 0, 0);
     }


     if(repaint_lives) {
        sp1_PrintAtInv(0, 8, INK_CYAN | PAPER_BLACK, 48 + lives);
        repaint_lives = 0;
     }

     sp1_UpdateNow();

     frame += 1;

     if(frame == 5) {
        frame = 0;
        print_points();
     }

     wait();
  }
}

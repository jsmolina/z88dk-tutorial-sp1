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
  speed = 6;
  nampac_go_home();
  repaint_lives = 1;

  ghost_red.offset = GHOST_RED;
  ghost_red.default_color =  initialiseColourGhostRed;
  ghost_red.default_y = 15;
  ghost_red.default_x = 14;

  ghost_cyan.offset = GHOST_CYAN;
  ghost_cyan.default_color =  initialiseColourGhostCyan;
  ghost_cyan.default_y = 15;
  ghost_cyan.default_x = 12;

  ghost_magenta.offset = GHOST_MAGENTA;
  ghost_magenta.default_color =  initialiseColourGhostMagenta;
  ghost_magenta.default_y = 15;
  ghost_magenta.default_x = 16;

  ghost_yellow.offset = GHOST_YELLOW;
  ghost_yellow.default_color =  initialiseColourYellow;
  ghost_yellow.default_y = 15;
  ghost_yellow.default_x = 18;


  reset_map();

  all_ghosts_go_home();


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

  ghost_red.sp = add_ghost_sprite();
  ghost_cyan.sp = add_ghost_sprite();
  ghost_magenta.sp = add_ghost_sprite();
  ghost_yellow.sp = add_ghost_sprite();
  cherry.sp = add_cherry_sprite();
  hide_cherry();

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
     if(cherry.showing != 0) {
         sp1_MoveSprAbs(cherry.sp, &full_screen, (void*) 0, cherry.y, cherry.x, 0, 0);
     }

     if(repaint_lives) {
        sp1_PrintAtInv(0, 8, INK_CYAN | PAPER_BLACK, 48 + lives);
        repaint_lives = 0;
     }

     sp1_UpdateNow();

     frame += 1;

     if(frame == 3) { // frame will go 0, 1, 2
        frame = 0;
        print_points();
     }

     wait();
  }
}

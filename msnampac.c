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
  level = 0;
  map_num = 1;
  nampac_go_home();
  repaint_lives = 1;


  ghost_cyan.offset = GHOST_CYAN;
  ghost_cyan.default_color =  initialiseColourGhostCyan;

  ghost_red.offset = GHOST_RED;
  ghost_red.default_color =  initialiseColourGhostRed;

  ghost_magenta.offset = GHOST_MAGENTA;
  ghost_magenta.default_color =  initialiseColourGhostMagenta;

  ghost_yellow.offset = GHOST_YELLOW;
  ghost_yellow.default_color =  initialiseColourYellow;

  set_ghosts_default_coords();

  reset_map();

  all_ghosts_go_home();


  sp1_UpdateNow();

  uint16_t has_kempston = in_stick_kempston();
  if(speed == 0) {
    show_billboard(READY);
  } else {
    show_billboard(GAME_OVER);
  }

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
  hide_billboard();
  pick = 1;
  speed = 6;
}


int main()
{
  setup_int();

  // now sp1
  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_BLACK,
                  ' ' );
  for (idx=0; idx != 10; idx++) {
      sp1_TileEntry(48+idx, numbers+idx*8);
  }

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

  sp1_TileEntry('j', bottomvertileft);

  sp1_TileEntry('s', terminal_left);
  sp1_TileEntry('t', terminal_right);
  sp1_TileEntry('u', terminal_up);
  sp1_TileEntry('v', terminal_down);

  sp1_TileEntry('w', union_left);
  sp1_TileEntry('x', union_right);
  sp1_TileEntry('y', door);
  sp1_TileEntry('z', vidas);

  pacman.sp = add_sprite();
  pacman.alt = add_dead_prota_sprite();
  pacman.offset = 1;
  pacman.currentoffset = 1;
  currentmap = &map[0][0];

  points_sp = add_points_sprite();
  billboard = add_billboard_sprite();
  ghost_red.sp = add_ghost_sprite();
  ghost_cyan.sp = add_ghost_sprite();
  ghost_magenta.sp = add_ghost_sprite();
  ghost_yellow.sp = add_ghost_sprite();
  cherry.sp = add_cherry_sprite();
  hide_cherry();

  // painting an UDG is just assigning it to any char
  // row, col, char

  // call it to initialize vars
  all_lives_lost();

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
         sp1_MoveSprAbs(cherry.sp, &full_screen, (void*) cherry.offset, cherry.y, cherry.x, 0, 0);
     }

     if(repaint_lives) {
        paint_lives();
        repaint_lives = 0;
     }

     if(showing_points != NONE) {
        --showing_points;
        if (showing_points == 0) {
            hide_points();
        }
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

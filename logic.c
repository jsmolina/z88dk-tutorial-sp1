#include "logic.h"
#include "int.h"
#include <sound.h>
#include <input.h>

uint8_t get_map_color(uint8_t current) {
    if(current == I) {
        return INK_WHITE;
    } else if (current == K) {
        return INK_MAGENTA;
    } else if (current == Y) {
        return INK_WHITE;
    }
    return INK_BLUE;
}

uint8_t get_map_char(uint8_t current) {

    if (current == 0)
    {
        return ' ';
    }

    return current + 96;
}

void reset_map() {
    for(row = 0; row != 24; ++row) {
      for(col = 0; col != 32; ++col) {
        if(map[row][col] == 18) {
             map[row][col] = 11;
        } else if(map[row][col] == 16) {
            map[row][col] = 9;
        }
        current = map[row][col];
        sp1_PrintAtInv(row, col, get_map_color(current) | INK_BLACK, get_map_char(current));
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
  // as points are restored...
  remaining_points = 238;

}

void show_cherry() {
    cherry.y = 21;
    cherry.x = 14;
    cherry.showing = 100;

    if(level == 0) {
        cherry.offset = CHERRY;
        sp1_IterateSprChar(cherry.sp, initialiseColourGreenRed);
    } else if(level == 1) {
        cherry.offset = STRAWBERRY;
        sp1_IterateSprChar(cherry.sp, initialiseColourGreenRed);
    } else if(level == 2) {
        cherry.offset = COCKTAIL;
        sp1_IterateSprChar(cherry.sp, initialiseColourRedYellowWhiteCyan);
    } else if(level == 3) {
        cherry.offset = APPLE;
        sp1_IterateSprChar(cherry.sp, initialiseColourGhostRed);
    } else if (level == 4) {
        cherry.offset = PEAR;
        sp1_IterateSprChar(cherry.sp, initialiseColourGreen);
    }
}

void hide_cherry() {
    cherry.y = 21;
    cherry.x = 32;
    cherry.showing = 0;
    sp1_MoveSprAbs(cherry.sp, &full_screen, (void*) 0, cherry.y, cherry.x, 0, 0);
}


void check_keys()
{
    // checks keys
    // allow jump in directions
    if ((in & IN_STICK_UP) && allow_next(map[row - 1][col])) {
        pacman.currentoffset = UP1;
        pacman.direction = DIR_UP;

    } else if((in & IN_STICK_DOWN) && allow_next(map[row + 1][col])) {
        pacman.currentoffset = DOWN1;
        pacman.direction = DIR_DOWN;
    }

    if((in & IN_STICK_LEFT) && allow_next(map[row][col - 1])) {
        pacman.currentoffset = LEFTC1;
        pacman.direction = DIR_LEFT;
    } else if((in & IN_STICK_RIGHT) && allow_next(map[row][col + 1])) {
        pacman.currentoffset = RIGHTC1;
        pacman.direction = DIR_RIGHT;
    }
}

void all_ghosts_go_home() {
    uint8_t i;
    for(i = 0; i != 4; ++i) {
        set_eaten(ghosts[i]);
    }
}

void nampac_go_home() {
    pacman.y = 21;
    pacman.x = 14;
    pacman.offset = RIGHTC1;
    pacman.direction = NONE;
}


void loose_a_live() {
    uint8_t i, j;
    hide_cherry();
    repaint_lives = 1;
    --lives;
    all_ghosts_go_home();

    // prota dead animation, first hide the sprite from the screen
    sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, 32, 0, 0);
    // 1, 33, 65, 97
    for(j = 1; j < 129; j += 32) {
        sp1_MoveSprAbs(pacman.alt, &full_screen, (void*) j, pacman.y, pacman.x, 0, 0);
        sp1_UpdateNow();
        for(i = 0; i != 5; ++i) {
            wait();
        }
    }

    sp1_MoveSprAbs(pacman.alt, &full_screen, (void*) col, pacman.y, 32, 0, 0);
    sp1_UpdateNow();

    nampac_go_home();
}

uint8_t allow_next(uint8_t next) {
    return next == 9 || next == 16 || next == 11 || next == 18;
}

// resets ghosts colours to default ones (when eaten, when finished ellude mode)
void reset_colors(struct sprite * for_who) {

    if(for_who->default_color == initialiseColourGhostRed) {
        for_who->currentoffset = GHOST_RED;
    } else if(for_who->default_color == initialiseColourGhostCyan) {
        for_who->currentoffset = GHOST_CYAN;
    } else if(for_who->default_color == initialiseColourGhostMagenta) {
        for_who->currentoffset = GHOST_MAGENTA;
    } else if(for_who->default_color == initialiseColourYellow) {
        for_who->currentoffset = GHOST_YELLOW;
    }
    sp1_IterateSprChar(for_who->sp, for_who->default_color);
    sp1_MoveSprAbs(for_who->sp, &full_screen, (void*) for_who->offset, for_who->default_y, for_who->default_x, 0, 0);
}

void set_eaten(struct sprite * for_who) {
    for_who->x = for_who->default_x;
    for_who->y = for_who->default_y;
    for_who->active = JAILED;
    for_who->dx = 0;
    for_who->dy = 0;
    reset_colors(for_who);
}


uint8_t goto_xy(struct sprite * for_who, uint8_t x, uint8_t y) {
    if(for_who->x != x) {
        if(for_who->x > x) {
            --for_who->x;
        } else if(for_who->x < x) {
            ++for_who->x;
        }
    } else {
        if(for_who->y > y) {
            --for_who->y;
        } else if(for_who->y < y) {
            ++for_who->y;
        } else {
            return ACTIVE;
        }
    }
    return JAILED_EXITING;
}

struct sprite * has_collision() {
    if(abs(pacman.x - ghosts[idx]->x) < 2 && abs(pacman.y - ghosts[idx]->y) < 2) {
        // eat
        return ghosts[idx];
    }
    return NULL;
}

uint8_t allow_next_ghost_pos(int8_t dy, int8_t dx) {
    row = ghosts[idx]->y + 1;
    return allow_next(map[row + dy][ghosts[idx]->x + dx]);
}

uint8_t could_go(uint8_t dir) {
    switch(dir) {
        case DIR_RIGHT:
            return allow_next(map[row][ghosts[idx]->x + 1]) && ghosts[idx]->dx != -1;
        break;
        case DIR_LEFT:
            return allow_next(map[row][ghosts[idx]->x - 1]) && ghosts[idx]->dx != 1;
        break;
        case DIR_DOWN:
            return allow_next(map[row + 1][ghosts[idx]->x]) && ghosts[idx]->dy != -1;
        break;
        case DIR_UP:
            return allow_next(map[row - 1][ghosts[idx]->x]) && ghosts[idx]->dy != 1;
        break;
    }
    return 0;
}

void remember_where_we_go(int8_t dy, int8_t dx) {
    ghosts[idx]->dx = dx;
    ghosts[idx]->dy = dy;
}

void then_go(uint8_t dir) {
    switch(dir) {
        case DIR_RIGHT:
            remember_where_we_go(0, 1);
            ghosts[idx]->x += 1;
            break;
        case DIR_LEFT:
            remember_where_we_go(0, -1);
            ghosts[idx]->x -= 1;
            break;
        case DIR_DOWN:
            remember_where_we_go(1, 0);
            ghosts[idx]->y += 1;
            break;
        case DIR_UP:
            remember_where_we_go(-1, 0);
            ghosts[idx]->y -= 1;
            break;
    }
}

void move_one_ghost(uint8_t t1, uint8_t t2, uint8_t t3, uint8_t t4) {
    // first check collisions
    if(ghosts[idx]->active == ELUDE) {
        // check collission before move
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            // eat but skip to be moved
            set_eaten(collided_sprite);
            return;
        }
    } else { // ACTIVE
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            loose_a_live();
            return;
        }
    }

    // then decide direction, first based on random params
    if((random_value < t1) && could_go(DIR_RIGHT)) {
        then_go(DIR_RIGHT);
        return;
    } else if((random_value < t2) && could_go(DIR_LEFT)) {
        then_go(DIR_LEFT);
        return;
    }

    if((random_value < t3) && could_go(DIR_DOWN)) {
        then_go(DIR_DOWN);
        return;
    } else if((random_value < t4) && could_go(DIR_UP)) {
        then_go(DIR_UP);
        return;
    }
    if(ghosts[idx]->active == ELUDE) {

        if(pacman.x < ghosts[idx]->x && could_go(DIR_RIGHT)) {
            then_go(DIR_RIGHT);
        } else if(pacman.x > ghosts[idx]->x && could_go(DIR_LEFT)) {
            then_go(DIR_LEFT);
        } else if(pacman.y < ghosts[idx]->y && could_go(DIR_DOWN)) {
            then_go(DIR_DOWN);
        } else if(pacman.y > ghosts[idx]->y && could_go(DIR_UP)) {
            then_go(DIR_UP);
        } else if(allow_next(map[row + ghosts[idx]->dy][ghosts[idx]->x + ghosts[idx]->dx])) {
            ghosts[idx]->x += ghosts[idx]->dx;
            ghosts[idx]->y += ghosts[idx]->dy;
        }

        // make ghost flicker if elude mode and almost finishing pill eaten
        if(pill_eaten < 40 && ghosts[idx]->active == ELUDE) {
            if((frame & 1) == 0) {
                sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
            } else {
                sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourWhite);
            }
        }

    } else { // ACTIVE

        if(pacman.x > ghosts[idx]->x && could_go(DIR_RIGHT)) {
            then_go(DIR_RIGHT);
        } else if(pacman.x < ghosts[idx]->x && could_go(DIR_LEFT)) {
            then_go(DIR_LEFT);
        } else if(pacman.y > ghosts[idx]->y && could_go(DIR_DOWN)) {
            then_go(DIR_DOWN);
        } else if(pacman.y < ghosts[idx]->y && could_go(DIR_UP)) {
            then_go(DIR_UP);
        } else if(allow_next(map[row + ghosts[idx]->dy][ghosts[idx]->x + ghosts[idx]->dx])) {
            ghosts[idx]->x += ghosts[idx]->dx;
            ghosts[idx]->y += ghosts[idx]->dy;
        }
    }

}

void move_ghosts() {
    // &ghost_red, &ghost_cyan, &ghost_magenta, &ghost_yellow
    row = ghosts[idx]->y + 1;

    switch(idx) {
        case 0: // Rojo: Intenta estár detras de Pac-Man en modo "Acoso"
            move_one_ghost(40, 80, 40, 100);
            break;
        case 1:
            // hasta que Pac-Man captura al menos 30 pildoras
            move_one_ghost(90, 120, 100, 160);
            break;
        case 2:
            // su comportamiento  siempre es llegar hacia el punto donde Pac-Man
            move_one_ghost(10, 20, 10, 20);
            break;
        case 3:
            // "a su bola"
            move_one_ghost(125, 255, 125, 255);
    }
}


void next_level() {
    zx_border(INK_BLUE);
    bit_beepfx_di_fastcall(BEEPFX_SCORE);
    zx_border(INK_BLACK);
    reset_map();
    repaint_lives = 1;
    nampac_go_home();
    all_ghosts_go_home();
    if (speed > 1) {
        --speed;
    }

    ++level;
    if(level == 5) {
        level = 0;
    }
}


void check_fsm() {
    random_value = rand();
    row = pacman.y + 1;
    if(row > 22) {
        row = 22;
    }
    col = pacman.x;
    if(col > 31) {
        col = 31;
    }
    // row and col must be set at this point
    check_keys();

    // pill eat
    current = map[row][col];
    if(current != 16 && current != 18) {
        map[row][col] = current + 7; // 9 + 7 = 16, 11 + 7 = 18
        sp1_PrintAtInv(row, col,  INK_BLACK, ' ');

        if(current == 9) {
            pick += 1;
            points += 5; // 5 points each dot
            --remaining_points;
        } else if(current == 11) {
            points += 20;  // energizers - are worth 20 points each
            pill_eaten = 125;
            for(idx = 0; idx != 4; ++idx) {
                if(ghosts[idx]->active == ACTIVE || ghosts[idx]->active == ELUDE) {
                    // stop so we could decide to "sacar pies en polvorosa"
                    ghosts[idx]->dx = 0;
                    ghosts[idx]->dy = 0;

                    ghosts[idx]->active = ELUDE;
                    ghosts[idx]->currentoffset = GHOST_FRIGHTENED;
                    sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
                }
            }
        }
    }

    // side change
    if(pacman.y == 12) {
        if(pacman.x < 2 && pacman.direction == DIR_LEFT) {
            pacman.x = 29;
        } else if(pacman.x > 28 && pacman.direction == DIR_RIGHT) {
            pacman.x = 1;
        }
    }

    if(pacman.direction == DIR_UP && allow_next(map[row - 1][col])) {
        --pacman.y;
    } else if(pacman.direction == DIR_DOWN && allow_next(map[row + 1][col])) {
        ++pacman.y;
    } else if(pacman.direction == DIR_LEFT && allow_next(map[row][col - 1])) {
        --pacman.x;
    } else if(pacman.direction == DIR_RIGHT && allow_next(map[row][col + 1])) {
        ++pacman.x;
    }

    if(frame == 0) {
        pacman.offset = pacman.currentoffset;
    } else if(frame == 1) {
        pacman.offset = pacman.currentoffset + 32;
    } else if(frame == 2) {
        pacman.offset = pacman.currentoffset + 64;
    }

    // IA FOR GHOSTS
    for(idx = 0; idx != 4; ++idx) {
        if(ghosts[idx]->active == JAILED_EXITING) {
            if(points > 300 || (points > 10 && idx != 1)) {
                ghosts[idx]->active = goto_xy(ghosts[idx], 15, 12);
            }
        } else if(ghosts[idx]->active == ACTIVE || ghosts[idx]->active == ELUDE) {
            move_ghosts();
        } else if(ghosts[idx]->active <= JAILED) {
            --ghosts[idx]->active;
        }
        // side change
        if(ghosts[idx]->y == 12) {
            if(ghosts[idx]->x < 2 && ghosts[idx]->dx == -1) {
                ghosts[idx]->x = 29;
            } else if(ghosts[idx]->x > 28 && ghosts[idx]->dx == 1) {
                ghosts[idx]->x = 1;
            }
        }

        if(frame == 0) {
            ghosts[idx]->offset = ghosts[idx]->currentoffset;
        } else if(frame == 1) {
            ghosts[idx]->offset = ghosts[idx]->currentoffset + 32;
        } else if(frame == 2) {
            ghosts[idx]->offset = ghosts[idx]->currentoffset;
        }
    }
    // while has eaten pill
    if(pill_eaten != NONE) {
        --pill_eaten;

    }

    if(pill_eaten == 0) {
        pill_eaten = NONE;
        for(idx = 0; idx != 4; ++idx) {
            if(ghosts[idx]->active == ELUDE) {
                ghosts[idx]->active = ACTIVE;
                reset_colors(ghosts[idx]);
            }
        }
        sp1_IterateSprChar(ghost_red.sp, initialiseColourGhostRed);
        sp1_IterateSprChar(ghost_cyan.sp, initialiseColourGhostCyan);
        sp1_IterateSprChar(ghost_magenta.sp, initialiseColourGhostMagenta);
        sp1_IterateSprChar(ghost_yellow.sp, initialiseColourYellow);
    }

    if(cherry.showing > 0) {
        if(pacman.x == cherry.x && pacman.y == cherry.y) {
            hide_cherry();
            points += 10;
        }
        --cherry.showing;
        if(cherry.showing == 0) {
            hide_cherry();
        }
    } else if(random_value == 200) {
        show_cherry();
        zx_border(INK_RED);
        zx_border(INK_BLACK);
    }

    if(remaining_points == 0) {
        // level finished!
        next_level();
    }

    if(in_key_pressed(IN_KEY_SCANCODE_1)) {
        sp1_MoveSprAbs(billboard, &full_screen, (void *) READY, 7, 12, 0, 0);
    } else if(in_key_pressed(IN_KEY_SCANCODE_2)) {
        level = 2;
        show_cherry();
    }else if(in_key_pressed(IN_KEY_SCANCODE_3)) {
        level = 3;
        show_cherry();
    }else if(in_key_pressed(IN_KEY_SCANCODE_4)) {
        level = 4;
        show_cherry();
    }
}

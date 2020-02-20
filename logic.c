#include "logic.h"
#include "int.h"
#include "game_zx.h"
#include <sound.h>
#include <input.h>
#include "mssound.h"


void show_billboard(uint8_t offset) {
    if(offset == READY) {
        sp1_IterateSprChar(billboard, initialiseColourYellow);
    } else {
        sp1_IterateSprChar(billboard, initialiseColourGhostRed);
    }

    sp1_MoveSprAbs(billboard, &full_screen, (void *) offset, 18, 11, 0, 0);
    sp1_UpdateNow();
}

void hide_billboard() {
    sp1_MoveSprAbs(billboard, &full_screen, (void *) READY, 18, 32, 0, 0);
}


uint8_t get_map_color(uint8_t current) {
    if(current == I) {
        return INK_WHITE;
    } else if (current == K) {
        return INK_MAGENTA;
    } else if (current == Y) {
        return INK_WHITE;
    }
    // INK_BLUE for level 1, INK_CYAN level2
    if(map_num == 3) {
        return INK_MAGENTA | BRIGHT;
    } else if(map_num == 2) {
        return INK_CYAN | BRIGHT;
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
      matrixrow = row * NCLS;
      for(col = 0; col != 32; ++col) {
        if(currentmap[matrixrow + col] == 18) {
            currentmap[matrixrow + col] = 11;
        } else if(currentmap[matrixrow + col] == 16) {
            currentmap[matrixrow + col] = 9;
        }
        current = currentmap[matrixrow + col]; // todo access currentmap
        sp1_PrintAtInv(row, col, get_map_color(current) | INK_BLACK, get_map_char(current));
      }

  }
  printatstr(0, 21, INK_WHITE | PAPER_BLACK, "1UP-");
  printatstr(0, 1, INK_WHITE | PAPER_BLACK, "LIVE-");
}

void set_ghosts_default_coords() {
    current = 12;
    for(idx = 0; idx != 4; ++idx) {
        ghosts[idx]->default_x = current;
        current = current + 2;

        if(map_num == 1) {
            ghosts[idx]->default_y = Y_GHOSTS_HOME_MAP1;
        } else if(map_num == 2) {
            ghosts[idx]->default_y = Y_GHOSTS_HOME_MAP2;
        } else if(map_num == 3) {
            ghosts[idx]->default_y = Y_GHOSTS_HOME_MAP3;
        }
    }
}

void show_cherry() {
    cherry.y = 21;
    cherry.x = 13;
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
        sp1_IterateSprChar(cherry.sp, initialiseColourRedApple);
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

void show_points(uint8_t y, uint8_t x) {
    sp1_MoveSprAbs(points_sp, &full_screen, (void*) POINTS_100, y, x, 0, 0);
    sp1_UpdateNow();
    showing_points = 10;
}

void hide_points() {
    sp1_MoveSprAbs(points_sp, &full_screen, (void*) POINTS_100, 32, 14, 0, 0);
}

void check_keys()
{
    // checks keys
    // allow jump in directions
    if ((in & IN_STICK_UP) && allow_next(currentmap[matrixrow - NCLS + col])) {
        pacman.currentoffset = UP1;
        pacman.direction = DIR_UP;

    } else if((in & IN_STICK_DOWN) && allow_next(currentmap[matrixrow + NCLS + col])) {
        pacman.currentoffset = DOWN1;
        pacman.direction = DIR_DOWN;
    }

    if((in & IN_STICK_LEFT) && allow_next(currentmap[matrixrow + (col - 1)])) {
        pacman.currentoffset = LEFTC1;
        pacman.direction = DIR_LEFT;
    } else if((in & IN_STICK_RIGHT) && allow_next(currentmap[matrixrow + (col + 1)])) {
        pacman.currentoffset = RIGHTC1;
        pacman.direction = DIR_RIGHT;
    }
}

void all_ghosts_go_home() {
    uint8_t i;
    for(i = 0; i != 4; ++i) {
        init_ghost(ghosts[i]);
    }
}

void nampac_go_home() {
    pacman.y = 21;
    pacman.x = 13;
    if (map_num == 1) {
        pacman.currentoffset = RIGHTC1;
        pacman.offset = RIGHTC1;
        pacman.direction = DIR_RIGHT;
    } else {
        pacman.currentoffset = UP1;
        pacman.offset = UP1;
        pacman.direction = DIR_UP;
    }
    sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, 2, 2, 0, 0);
    sp1_MoveSprAbs(pacman.sp, &full_screen, (void*) pacman.offset, pacman.y, pacman.x, 0, 0);
}


void loose_a_live() {
    uint8_t i, j;
    hide_cherry();
    hide_points();
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
    if(lives > 0) {
        show_billboard(READY);
    } else {
        show_billboard(GAME_OVER);
    }
    in_wait_key();
    hide_billboard();

}

uint8_t allow_next(uint8_t next) {
    return next == 9 || next == 16 || next == 11 || next == 18;
}

// resets ghosts colours to default ones (when eaten, when finished ellude mode)
void reset_colors(struct spritep * for_who) {

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

void set_eaten(struct spritep * for_who) {
    for_who->active = GETTING_JAILED;
    for_who->offset = GHOST_EYES;
    for_who->direction = NONE;
    for_who->last_dir = NONE;
    show_points(for_who->y, for_who->x);
    reset_colors(for_who);
}

void init_ghost(struct spritep * for_who) {
    for_who->x = for_who->default_x;
    for_who->y = for_who->default_y;
    for_who->active = JAILED;
    for_who->direction = NONE;
    for_who->last_dir = NONE;
    reset_colors(for_who);

    // ensure repaint: sp1 tricky
    sp1_MoveSprAbs(for_who->sp, &full_screen, (void*) for_who->offset, 2, 2, 0, 0);
    sp1_MoveSprAbs(for_who->sp, &full_screen, (void*) for_who->offset, for_who->y, for_who->x, 0, 0);
}


uint8_t goto_xy(struct spritep * for_who, uint8_t x, uint8_t y) {
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
            return CHASE;
        }
    }
    return JAILED_EXITING;
}

struct spritep * has_collision() {
    if(abs(pacman.x - ghosts[idx]->x) < 2 && abs(pacman.y - ghosts[idx]->y) < 2) {
        // eat
        return ghosts[idx];
    }
    return NULL;
}


uint8_t could_go(uint8_t dir) {
    switch(dir) {
        case DIR_RIGHT:
            return allow_next(currentmap[matrixrow_ghost + (ghosts[idx]->x + 1)]) && ghosts[idx]->last_dir != DIR_LEFT;
        break;
        case DIR_LEFT:
            return allow_next(currentmap[matrixrow_ghost + (ghosts[idx]->x - 1)]) && ghosts[idx]->last_dir != DIR_RIGHT;
        break;
        case DIR_DOWN:
            return allow_next(currentmap[(matrixrow_ghost + NCLS) + ghosts[idx]->x]) && ghosts[idx]->last_dir != DIR_UP;
        break;
        case DIR_UP:
            return allow_next(currentmap[(matrixrow_ghost - NCLS) + ghosts[idx]->x]) && ghosts[idx]->last_dir != DIR_DOWN;
        break;
    }
    return 0;
}

void then_go(uint8_t dir) {
    ghosts[idx]->direction = dir;
    ghosts[idx]->last_dir = dir;
}

uint8_t move_ghost_in_his_direction() {
    if(!could_go(ghosts[idx]->direction)) {
        ghosts[idx]->direction = NONE;
        return 0;
    }

    switch(ghosts[idx]->direction) {
        case DIR_RIGHT:
            ghosts[idx]->x += 1;
            break;
        case DIR_LEFT:
            ghosts[idx]->x -= 1;
            break;
        case DIR_DOWN:
            ghosts[idx]->y += 1;
            break;
        case DIR_UP:
            ghosts[idx]->y -= 1;
            break;
    }
    return 1;
}

void choose_random_direction() {
    tmp_val = ghosts[idx]->last_dir;
    while(!(could_go(tmp_val))) {
        tmp_val = (rand() & 3) + 1;
    }
    then_go(tmp_val);
}

uint16_t euclidean_dist(int8_t x, int8_t y, int8_t px, int8_t py) {
    x = px - x;
    y = py - y;
    return (x * x) + (y * y);
}
/**
px is targeted x coord, while py is targeted y coord
**/
uint8_t ghost_gotoIA(uint8_t px, uint8_t py) {
    uint16_t mincost = 65500;
    uint16_t foundcost;
    // keep current if not found
    uint8_t chosen = ghosts[idx]->direction;

    // si suma + 1 en la direccion que va a tomar, deberia ser la distancia
    // mas pequeña
    // si misma distancia, primero arriba, izquierda, abajo, derecha
    if(could_go(DIR_UP)) {
        mincost = euclidean_dist(ghosts[idx]->x, ghosts[idx]->y - 1, px, py);
        chosen = DIR_UP;
    }
    if (could_go(DIR_LEFT)) {
        foundcost = euclidean_dist(ghosts[idx]->x - 1, ghosts[idx]->y, px, py);
        if (foundcost < mincost) {
            mincost = foundcost;
            chosen = DIR_LEFT;
        }
    }
    if (could_go(DIR_DOWN)) {
        foundcost = euclidean_dist(ghosts[idx]->x, ghosts[idx]->y + 1, px, py);
        if (foundcost < mincost) {
            mincost = foundcost;
            chosen = DIR_DOWN;
        }
    }
    if (could_go(DIR_RIGHT)) {
        foundcost = euclidean_dist(ghosts[idx]->x + 1, ghosts[idx]->y, px, py);
        if (foundcost < mincost) {
            chosen = DIR_RIGHT;
        }
    }

    return chosen;
}

void move_one_ghost() {
    matrixrow_ghost = (ghosts[idx]->y + 1) * NCLS;

    // make ghost flicker if elude mode and almost finishing pill eaten
    if(pill_eaten < 40 && ghosts[idx]->active == FRIGHTENED) {
        if((frame & 1) == 0) {
            sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
        } else {
            sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourWhite);
        }
    }


    if(ghosts[idx]->active == GETTING_JAILED) {
        if(ghosts[idx]->default_y == ghosts[idx]->y && ghosts[idx]->default_x == ghosts[idx]->x) {
            ghosts[idx]->active = JAILED;
            return;
        }

        if(ghosts[idx]->default_y  > ghosts[idx]->y) {
            ++ghosts[idx]->y;
        } else if(ghosts[idx]->default_y  < ghosts[idx]->y ) {
            --ghosts[idx]->y;
        }

        if(ghosts[idx]->default_x  > ghosts[idx]->x  ) {
            ++ghosts[idx]->x;
        } else if(ghosts[idx]->default_x  < ghosts[idx]->x ) {
            --ghosts[idx]->x;
        }


        return;
    }

    // first check collisions
    if(ghosts[idx]->active == FRIGHTENED) {
        // check collission before move
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            // eat but skip to be moved
            set_eaten(collided_sprite);
            return;
        }
    } else { // CHASE or SCATTER
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            loose_a_live();
            return;
        }
    }

    if(ghosts[idx]->active == FRIGHTENED) {
        // direction constants go from 1 to 4
        tmp_val = ghosts[idx]->last_dir;
        while(!(could_go(tmp_val))) {
            tmp_val = (rand() & 3) + 1;
        }
        then_go(tmp_val);

    } else if(ghosts[idx]->active == CHASE) {

        if (idx == GRED) {
            // blinky is directly targeting pacman
            then_go(ghost_gotoIA(pacman.x, pacman.y));
        } else if (idx == GMAGENTA) {
            // Pinky four tiles in front of pacman
            tmp_val = pacman.x;
            tmp_val2 = pacman.y;

            if(pacman.direction == DIR_LEFT) {
                tmp_val -= 4;
            } else if (pacman.direction == DIR_RIGHT) {
                tmp_val += 4;
            } else if (pacman.direction == DIR_UP) {
                tmp_val -= 4;
                tmp_val2 -= 4;
            } else {
                tmp_val2 += 4;
            }
            then_go(ghost_gotoIA(tmp_val, tmp_val2));
        } else if(idx == GCYAN) {
            tmp_val = pacman.x;
            tmp_val2 = pacman.y;
            if(pacman.direction == DIR_LEFT) {
                tmp_val -= 2;
            } else if (pacman.direction == DIR_RIGHT) {
                tmp_val += 2;
            } else if (pacman.direction == DIR_UP) {
                tmp_val -= 2;
                tmp_val2 -= 2;
            } else {
                tmp_val2 += 2;
            }
            // Inky has 180 degrees over RED (blinky) target vector!
            then_go(ghost_gotoIA(tmp_val - ghost_red.x, tmp_val2 - ghost_red.y));
        } else if(idx == GYELLOW) {
           //Clyde
           // go chase, except when close, so then x=0, y= 24
           if (abs(ghosts[idx]->x - pacman.x) > 4 && abs(ghosts[idx]->y - pacman.y) > 4) {
                then_go(ghost_gotoIA(pacman.x, pacman.y));
           } else {
                // if so close, scatter
                then_go(ghost_gotoIA(0, 24));
           }
        }

    } else if(ghosts[idx]->active == SCATTER) {

        if(idx == GCYAN) {
            then_go(ghost_gotoIA(32, 24));
        } else if (idx == GRED) {
            then_go(ghost_gotoIA(32, 0));
        } else if (idx == GMAGENTA) {
            then_go(ghost_gotoIA(0, 0));
        } else if (idx == GYELLOW) {
            then_go(ghost_gotoIA(0, 24));
        }
    }


    if(ghosts[idx]->direction != NONE) { // not found already a collision
        if (ghosts[idx]->active == FRIGHTENED && (((frame + idx) & 1) == 0)) {
            // no move for altern frames: make ghost slower
            return;
        }
        move_ghost_in_his_direction();
    }

}

void move_ghosts() {
    // &ghost_red, &ghost_cyan, &ghost_magenta, &ghost_yellow

    switch(idx) {
        case GCYAN:
            move_one_ghost();
            break;
        case GRED:
            move_one_ghost();
            break;

        case GMAGENTA:
            move_one_ghost();
            break;

        case GYELLOW:
            move_one_ghost();
    }

    // switches to scatter or chase, except blinky that depends on number of remaining points
    if (idx == GRED ) {
        if(reached_level == 0 && remaining_points < 20) {
            return;
        } else if(reached_level == 1 && remaining_points < 30) {
            return;
        } else if(reached_level >= 2 && reached_level <= 5 && remaining_points < 40) {
            return;
        } else if(reached_level >= 6 && reached_level <= 8 && remaining_points < 50) {
            return;
        } else if(reached_level >= 9 && reached_level <= 11 && remaining_points < 60) {
            return;
        } else if(reached_level >= 12 && reached_level <= 14 && remaining_points < 80) {
            return;
        } else if(reached_level > 14  && remaining_points < 120) {
            // blinky almost always chase after passing 14 times!
            return;
        }
    }

    if((ghosts[idx]->active == CHASE || ghosts[idx]->active == SCATTER)) {

        if(idx == GCYAN && frame == 1) { // only advances if ghosts are actively moving and for one ghost
            ++slowticker;
        }

        if(slowticker == 8) {
            // go scatter
            ghosts[idx]->active = SCATTER;
        } else if(slowticker == 30) {
            ghosts[idx]->active = CHASE;

        } else if(slowticker == 38) {
            ghosts[idx]->active = SCATTER;
        } else if(slowticker == 55) {
            ghosts[idx]->active = CHASE;

        } else if(slowticker == 63) {
            ghosts[idx]->active = SCATTER;
        } else if(slowticker == 85) {
            ghosts[idx]->active = CHASE;

        } else if((reached_level < 3 && slowticker == 91) || (reached_level > 3 && slowticker == 155) ) {
            ghosts[idx]->active = SCATTER;
        } else if(slowticker == 160 || slowticker == 96) {
            ghosts[idx]->active = CHASE;
        }
    }


}


void next_level() {
    zx_border(INK_BLUE);
    bit_beepfx_di_fastcall(BEEPFX_SCORE);
    zx_border(INK_BLACK);
    ++level;
    // helps determining scatter mode changes and some others
    slowticker = 0;
    ++reached_level;
    ++map_num;
    if(map_num > 3) {
        map_num = 1;
    }

    if(map_num == 1) {
        remaining_points = MAP1_TOTAL_POINTS;
    } else if(map_num == 2){
        remaining_points = MAP2_TOTAL_POINTS;
    } else if(map_num == 3){
        remaining_points = MAP3_TOTAL_POINTS;
    }


    if(map_num == 1) {
        currentmap = &map[0][0];
    } else if(map_num == 2) {
        currentmap = &map2[0][0];
    } else {
        currentmap = &map3[0][0];
    }

    reset_map();
    set_ghosts_default_coords();

    repaint_lives = 1;
    nampac_go_home();
    all_ghosts_go_home();

    if(level == 5) {
        level = 0;
    }

    show_billboard(READY);
    in_wait_key();
    hide_billboard();
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
    if(points > 65500) {
        if(lives < 5) {
            ++lives;
            bit_beepfx_di_fastcall(BEEPFX_BOOM_1);
        }
    }
    // row and col must be set at this point
    matrixrow = row * NCLS;
    check_keys();

    // pill eat
    current = currentmap[matrixrow + col];
    if(current != 16 && current != 18) {
        currentmap[matrixrow + col] = current + 7; // 9 + 7 = 16, 11 + 7 = 18
        sp1_PrintAtInv(row, col,  INK_BLACK, ' ');

        if(current == 9) {
            pick += 1;
            sounds_waka();
            points += 5; // 5 points each dot
            --remaining_points;
        } else if(current == 11) {
            points += 20;  // energizers - are worth 20 points each
            pill_eaten = 90;
            for(idx = 0; idx != 4; ++idx) {
                if((ghosts[idx]->active == CHASE || ghosts[idx]->active == FRIGHTENED
                || ghosts[idx]->active == SCATTER) && level < 19) {
                    // "sacar pies en polvorosa"
                    if(ghosts[idx]->direction == DIR_LEFT) {
                        then_go(DIR_RIGHT);
                    } else if(ghosts[idx]->direction == DIR_RIGHT) {
                        then_go(DIR_LEFT);
                    } else if(ghosts[idx]->direction == DIR_UP) {
                        then_go(DIR_DOWN);
                    } else {
                        then_go(DIR_UP);
                    }

                    ghosts[idx]->active = FRIGHTENED;
                    ghosts[idx]->currentoffset = GHOST_FRIGHTENED;
                    sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
                }
            }
        }
    }

    // side change
    if((map_num == 1 && pacman.y == MAP1_Y_SIDE_CHG) ||
        (map_num == 2 && pacman.y == MAP2_Y_SIDE_CHG) ||
        (map_num == 3 && pacman.y == MAP3_Y_SIDE_CHG)) {
        if(pacman.x < 2 && pacman.direction == DIR_LEFT) {
            pacman.x = 30;
        } else if(pacman.x >= 29 && pacman.direction == DIR_RIGHT) {
            pacman.x = 1;
        }
    }

    if(pacman.direction == DIR_UP && allow_next(currentmap[matrixrow - NCLS + col])) {
        --pacman.y;
    } else if(pacman.direction == DIR_DOWN && allow_next(currentmap[matrixrow + NCLS + col])) {
        ++pacman.y;
    } else if(pacman.direction == DIR_LEFT && allow_next(currentmap[matrixrow + (col - 1)])) {
        --pacman.x;
    } else if(pacman.direction == DIR_RIGHT && allow_next(currentmap[matrixrow + (col + 1)])) {
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
        random_value = rand();
        if(ghosts[idx]->active == JAILED_EXITING) {
            if(points > 300 || (points > 10 && idx != 1)) {
                ghosts[idx]->active = goto_xy(ghosts[idx], 15, ghosts[idx]->default_y - 3);
            }
        } else if(ghosts[idx]->active == CHASE || ghosts[idx]->active == FRIGHTENED
            || ghosts[idx]->active == GETTING_JAILED || ghosts[idx]->active == SCATTER) {
            move_ghosts();
        } else if(ghosts[idx]->active <= JAILED && random_value < 100) {
            --ghosts[idx]->active;
        }
        // side change
        if((map_num == 1 && ghosts[idx]->y == MAP1_Y_SIDE_CHG) ||
            (map_num == 2 && ghosts[idx]->y == MAP2_Y_SIDE_CHG)||
            (map_num == 3 && ghosts[idx]->y == MAP3_Y_SIDE_CHG)) {
            if(ghosts[idx]->x < 2 && ghosts[idx]->direction == DIR_LEFT) {
                ghosts[idx]->x = 29;
            } else if(ghosts[idx]->x > 28 && ghosts[idx]->direction == DIR_RIGHT) {
                ghosts[idx]->x = 1;
            }
        }

        if(ghosts[idx]->active != GETTING_JAILED) {
            if(frame == 0 || frame == 2) {
                ghosts[idx]->offset = ghosts[idx]->currentoffset;
            } else if(frame == 1) {
                ghosts[idx]->offset = ghosts[idx]->currentoffset + 32;
            }
        }
    }
    // while has eaten pill
    if(pill_eaten != NONE) {
        --pill_eaten;

    }

    if(pill_eaten == 0) {
        pill_eaten = NONE;
        for(idx = 0; idx != 4; ++idx) {
            if(ghosts[idx]->active == FRIGHTENED) {
                ghosts[idx]->active = CHASE;
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
            bit_beep(10, 1200);
            zx_border(INK_BLACK);
            show_points(21, 14);
            points += 100;
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

}

void paint_lives() {
    for(idx = 0; idx != 5; ++idx) {
        if(idx < lives) {
            sp1_PrintAtInv(0, 6 + idx, INK_YELLOW | PAPER_BLACK | BRIGHT, 'z');
        } else {
            sp1_PrintAtInv(0, 6 + idx, get_map_color(0) | PAPER_BLACK, 'a');
        }
    }
}
#include "logic.h"
#include "int.h"
#include <sound.h>


void show_cherry() {
    cherry.y = 21;
    cherry.x = 14;
    cherry.showing = 100;
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
        pacman.dy = -1;
        pacman.currentoffset = UP1;

    } else if((in & IN_STICK_DOWN) && allow_next(map[row + 1][col])) {
        pacman.dy = 1;
        pacman.currentoffset = DOWN1;
    }

    if((in & IN_STICK_LEFT) && allow_next(map[row][col - 1])) {
        pacman.dx = -1;
        pacman.currentoffset = LEFTC1;
    } else if((in & IN_STICK_RIGHT) && allow_next(map[row][col + 1])) {
        pacman.dx = 1;
        pacman.currentoffset = RIGHTC1;
    }
}

void all_ghosts_go_home() {
    uint8_t i;
    for(i = 0; i != 4; ++i) {
        set_eaten(ghosts[i]);
        sp1_MoveSprAbs(ghosts[i]->sp, &full_screen, (void*) ghosts[i]->offset, ghosts[i]->default_y, ghosts[i]->default_x, 0, 0);
    }
}

void nampac_go_home() {
    pacman.y = 21;
    pacman.x = 14;
    pacman.dx = 0;
    pacman.dy = 0;
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

void set_eaten(struct sprite * for_who) {
    for_who->x = for_who->default_x;
    for_who->y = for_who->default_y;
    for_who->active = JAILED;
    for_who->dx = 0;
    for_who->dy = 0;
    sp1_IterateSprChar(for_who->sp, for_who->default_color);
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
        // check collission before move
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            // eat
            set_eaten(collided_sprite);
        }

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
        if(pill_eaten < 40 && ghosts[idx]->active == ACTIVE) {
            if((frame & 1) == 0) {
                sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
            } else {
                sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourWhite);
            }
        }

    } else { // ACTIVE
        collided_sprite = has_collision();
        if(collided_sprite != NULL) {
            loose_a_live();
            return;
        }

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
        case 0: // Rojo: Intenta estár detrás de Pac-Man en modo "Acoso"
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
    remaining_points = 238;
    nampac_go_home();
    all_ghosts_go_home();
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
                if(ghosts[idx]->active == ACTIVE) {
                    ghosts[idx]->active = ELUDE;
                    sp1_IterateSprChar(ghosts[idx]->sp, initialiseColourBlue);
                }
            }
        }
    }

    // side change
    if(pacman.y == 12) {
        if(pacman.x < 2 && pacman.dx == -1) {
            pacman.x = 29;
        } else if(pacman.x > 28 && pacman.dx == 1) {
            pacman.x = 1;
        }
    }

    if(ghosts[frame]->y == 12) {
        if(ghosts[frame]->x < 2 && ghosts[frame]->dx == -1) {
            ghosts[frame]->x = 29;
        } else if(ghosts[frame]->x > 28 && ghosts[frame]->dx == 1) {
            ghosts[frame]->x = 1;
        }
    }


    if(allow_next(map[row + pacman.dy][col + pacman.dx])) {
        pacman.y += pacman.dy;
        pacman.x += pacman.dx;
    } else if (pacman.dy != 0) {
        pacman.dy = 0;
        pacman.dx = 0;
    }

    if((tick & 1) == 0) {
        pacman.offset = pacman.currentoffset + 32;
        ghosts[frame]->offset = ghosts[frame]->currentoffset + 32;
    } else {
        pacman.offset = pacman.currentoffset;
        ghosts[frame]->offset = ghosts[frame]->currentoffset;
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
}


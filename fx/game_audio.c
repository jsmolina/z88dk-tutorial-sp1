/*
        This file is part of NamCap.

        NamCap is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        NamCap is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with NamCap.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "game_audio.h"


void audio_eat_dot(void) {
    __asm
        EXTERN MFXADD
        EXTERN _ay_fx_eat1
        ld hl, _ay_fx_eat1
        call MFXADD
    __endasm;
}

void audio_add_fruit() {
    //ay_fx_play_mfx(4, ay_fx_fruit_target);
}

void audio_eat_fruit() {
    //ay_fx_play_mfx(4, ay_fx_fruit_eat);
}

void audio_eat_pill() {
    //ay_fx_play_mfx(4, ay_fx_fruit_eat);
}

void audio_eat_ghost() {
  //ay_fx_play_mfx(4, ay_fx_ghost_eat);
}

void audio_ay_death() {
    __asm
        EXTERN MFXADD
        EXTERN _ay_fx_death
        ld hl, _ay_fx_death
        call MFXADD
    __endasm;
}

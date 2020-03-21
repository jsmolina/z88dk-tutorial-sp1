#include "mssound.h"

extern wyz_effects myeffects;
//extern wyz_song mysong;

void sounds_init() {
    //ay_wyz_init(&mysong);
    ay_wyz_effect_init(&myeffects);
}

void sounds_waka() {
    //TODO ay_wyz_start_effect(3, 0);
}

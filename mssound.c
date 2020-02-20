#include "sound.h"

extern wyz_effects myeffects;

void sounds_init() {
    ay_wyz_effect_init(&myeffects);
}

void sounds_waka() {
    ay_wyz_start_effect(3, 0);
}

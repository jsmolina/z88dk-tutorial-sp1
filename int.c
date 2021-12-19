#include <z80.h>
#include "int.h"
#include "globals.h"
#include <stdlib.h>
#include <intrinsic.h>

unsigned char tick;
unsigned char timer;
unsigned char pick;
unsigned char music_playing;



void
wait(void)
{
   while (abs(tick - timer) < SPEED) {
      intrinsic_halt();
      in = (joy)(&joy_keys);
   }

   timer = tick;
}
// hay 2 efectos para comer bolas, porque lleva un contaje de las bolas
// segun ese número sea par o impar lanza un efecto de comer o el otro
// en el pacman original hay 4 frames de separación entre cada efecto de
// comer una bola. Los fx de comer bola son 5 frames.
// Por tanto entre que come una bola y la siguiente son 9 frames
//  revisa el bit0 del número
//el tema de las sirenas también te lo pasé, según el numero de bolas
//que come pasa a una sirena u otra
//  el insert coin lo tiene en el canal A (igual que la alarma Tin Tin),
// ya que así siempre suena durante el juego ... y no interfiere
// en el sonido de sirena, por ejemplo
// el sonido de sirena y de huida de fantasma, lo tiene siempre en el canal B

// y el sonido de comer lo tiene en el canal C, igual que cuando come fruta
// o come un fastasma
// La función que he hecho de InitFx, tienes que indicar en qué canal lo lanzas,
// ya que hay efectos que tienen loop (las sirenas)



void start_ay() {
__asm
    extern startMusic
    extern enable_bank_n
    extern restore_bank_0
    ld a, 6
    call enable_bank_n
     call startMusic
    call restore_bank_0
__endasm;
}

void stop_ay() {
__asm
    extern MuteAy
    extern enable_bank_n
    extern restore_bank_0
    ld a, 6
    call enable_bank_n
     call MuteAy
    call restore_bank_0
__endasm;
}

void sonidoSirena() {
// todo variarla segun bolas comidas
__asm
    extern fxSirena1
    extern Load_Fx
    extern enable_bank_n
    extern restore_bank_0
    ld a, 6
    call enable_bank_n
      ld a, 2
	  ld hl, fxSirena1
	  call Load_Fx
    call restore_bank_0
__endasm;
}

void sonidoHuida() {
__asm
    extern fxHuidaFantasmas
    extern Load_Fx
    extern enable_bank_n
    extern restore_bank_0
    ld a,6
    call enable_bank_n
      ld a, 2
	  ld hl, fxHuidaFantasmas
	  call Load_Fx
    call restore_bank_0
__endasm;
}

void eat_ball_sound() {
__asm
    extern _pill_eaten
    extern playBall
    extern enable_bank_n
    extern restore_bank_0
    ld e, _pill_eaten
    ld a, 6
    call enable_bank_n
        call playBall
    call restore_bank_0
__endasm;
}

void loose_a_live_sound() {
__asm
    extern fxMuerte
    extern Load_Fx
    extern enable_bank_n
    extern restore_bank_0
    ld a,6
    call enable_bank_n
      ld a, 2
	  ld hl, fxMuerte
	  call Load_Fx
    call restore_bank_0
__endasm;
}


void eat_ghost_sound() {
__asm
    extern fxComeFantasma
    extern Load_Fx
    extern enable_bank_n
    extern restore_bank_0
    ld a,6
    call enable_bank_n
      ld a, 1
      ; fxOjosACasa
	  ld hl, fxComeFantasma
	  call Load_Fx
    call restore_bank_0
__endasm;
}


void eat_fruit_sound() {
__asm
    extern fxComeFruta
    extern Load_Fx
    extern enable_bank_n
    extern restore_bank_0
    ld a,6
    call enable_bank_n
      ld a, 1
      ; fxOjosACasa
	  ld hl, fxComeFruta
	  call Load_Fx
    call restore_bank_0
__endasm;
}

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
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
     call startMusic
    call restore_bank_0
__endasm;
}

void stop_ay() {
__asm
    extern MuteAy
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
     call MuteAy
    call restore_bank_0
__endasm;
}

void sonido2Sirena() {
__asm
    extern playSirena
    extern enable_bank_6
    extern restore_bank_0
    call enable_bank_6
      call playSirena
    call restore_bank_0
__endasm;
}

void incSiren() {
__asm
    extern enable_bank_6
    extern restore_bank_0
    extern playSirena
    extern FxStop
    extern incSiren
    call enable_bank_6
      call incSiren
      ld a, 2
      call FxStop
      call playSirena
    call restore_bank_0
__endasm;
}

void resetSiren() {
__asm
    extern enable_bank_6
    extern restore_bank_0
    extern resetSiren
    call enable_bank_6
      call resetSiren
    call restore_bank_0
__endasm;
}

void stopCanal3() {
    __asm
    extern FxStop
    call enable_bank_6
      ld a, 3
	  call FxStop
    call restore_bank_0
    __endasm;
}

void stopCanal2() {
    __asm
    extern FxStop
    call enable_bank_6
      ld a, 2
	  call FxStop
    call restore_bank_0
    __endasm;
}



void sonido2Huida() {
__asm
    extern fxHuidaFantasmas
    extern Load_Fx
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
      ld a, 2
      call FxStop
      ld a, 2
	  ld hl, fxHuidaFantasmas
	  call Load_Fx
    call restore_bank_0
__endasm;
}

void sonido1Bola() {
__asm
    extern playBall
    extern enable_bank_6
    extern restore_bank_0
    call enable_bank_6
        call playBall
    call restore_bank_0
__endasm;
}

void sonido3PierdeVida() {
__asm
    extern fxMuerte
    extern Load_Fx
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
      ld a, 3
	  ld hl, fxMuerte
	  call Load_Fx
    call restore_bank_0
__endasm;
}

void sonido3InsertCoin() {
__asm
    extern fxInsert_coin
    extern Load_Fx
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
      ld a, 3
	  ld hl, fxInsert_coin
	  call Load_Fx
    call restore_bank_0
__endasm;
}

void sonido13ComeGhost() {
__asm
    extern fxComeFantasma
    extern Load_Fx
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
      ld a, 1
	  ld hl, fxComeFantasma
	  call Load_Fx
      ld a, 3
	  ld hl, fxOjosACasa
	  call Load_Fx
    call restore_bank_0
__endasm;
}


void sonido1Fruta() {
__asm
    extern fxComeFruta
    extern fxOjosACasa
    extern Load_Fx
    extern enable_bank_6
    extern restore_bank_0
    ;ld a, 6
    call enable_bank_6
      ld a, 1
	  ld hl, fxComeFruta
	  call Load_Fx
    call restore_bank_0
__endasm;
}

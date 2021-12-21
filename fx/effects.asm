SECTION BANK_6

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;  TinyPlayer as PacMan style ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;by Sergio thEpOpE - 2021
;
;CreativeCommons CC BY
;
PUBLIC	startMusic
PUBLIC Load_Fx
PUBLIC playBall
PUBLIC playSirena
PUBLIC waitMusicEnd
PUBLIC FxStop
PUBLIC incSiren
PUBLIC resetSiren

EXTERN _pill_eaten
EXTERN enable_bank_n
EXTERN restore_bank_0

startMusic:
    ld hl, mInitGameA
	ld de, mInitGameB
	call Load_Music
	ret

waitMusicEnd:
    ; bucle infinito hasta que pare
    while:
    ld a, (playing_mus)
    cp 0
    jr nz, while
    ret

incSiren:
    ld a, (current_siren)
    inc a
    ld (current_siren), a
    ret

resetSiren:
    ld a, 1
    ld (current_siren), a
    ret

IsFxPlayin:
  push hl
  ld hl, canal_A_fx_estado + 2
  ld a,(hl)
  ld hl, canal_B_fx_estado + 2
  or (hl)
  ld hl, canal_C_fx_estado + 2
  or (hl)
  pop hl
  ret

;
;Para cargar una musica
; en HL el puntero al Track1 del canal A
; en DE el puntero al Track2 del canal B
;
; Call Load_Music


;Para cargar un efecto
; en HL el puntero a la definición del puntero
; en A el canal donde se quiere lanzar (1, 2, o 3)
;
; call Load_Fx


; Para cambiar la musica o el efecto en un canal no es necesario detenerlo
; basta cargar una nueva musica o un nuevo efecto en el mismo canal



;Para detener la música (se mutea el AY inmediatamente)
;
; call MusicStop

;Para detener los efectos de un canal
; en A se pone el numero de canal (1..3)
;
; call FxStop


;En cada interrupción se debería tener
;
; Call FxPlay
; Call MusicPlay
; Call UpdateAy
;
;
;para saber si la musica ha terminado de sonar, puedes verificar la
; variable playing_mus
; https://wikiti.brandonw.net/index.php?title=Z80_Optimization#Look_up_Table

playBall:
    push hl
    ld hl, fxComeBola_1
    ld a, (current_bola)
    bit 0, a ; test for bit 0
    jr z, cont
    ld hl, fxComeBola_2
    cont:
      ld a, 1
      call Load_Fx
    ld a, (current_bola) ; get value
    inc a ; will switch last bit (00, 01, 10, 11, ...)
    ld (current_bola), a ; switch value
    pop hl
    ret

playSirena:
      ; current_siren (1,2,3,4,5)
      ld a, (current_siren)
      cp 5
      jr c, lessThan5
      ld hl, fxSirena5
      jr endIfStatement
      lessThan5:
        cp 4
        jr c, lessThan4
        ld hl, fxSirena4
        jr endIfStatement
        lessThan4:
          cp 3
          jr c, lessThan3
          ld hl, fxSirena3
          jr endIfStatement
          lessThan3:
            cp 2
            jr c, lessThan2
            ld hl, fxSirena2
            jp endIfStatement
            lessThan2:
              ld hl, fxSirena1
      endIfStatement:
      ld a, 2
      call FxStop
      ld a, 2
	  call Load_Fx
	  ret

;;;
;;Load Music : Inicializa la tabla del player con los tracks en el canal A y B
;;
;;HL debe contener el offset al track del Canal A
;;DE debe contener el offset al track del Canal B
;;
Load_Music:
	push ix
	ld ix, canal_A_estado
	ld (ix+0),l
	ld (ix+1),h
	ld ix, canal_B_estado
	ld (ix+0),e
	ld (ix+1),d
	ld a,1
	ld (playing_mus),a
	pop ix
	ret

;;;
;;Load Fx : Iniciliza la tabla del player con un efecto en un canal
;;
;;En A va el numero de canal (1,2 o 3)
;;En HL el offset a la definicion del efecto

Load_Fx:
	push bc
	push ix
	ld bc,size_canalfx_estado
	ld ix,canal_A_fx_estado - size_canalfx_estado
sumamas:
	add ix,bc
	dec a
	jr nz, sumamas
	ld (ix+6),a	;por defecto el volumen no cae
	ld (ix+3),a ;numero de fines a leer 0, para procesar primero como código de control
	ld (ix+0),l	;IX apunta a la tabla del canal de efectos
	ld (ix+1),h
	inc a		;aqui llegamos con A=0
	ld (ix+2),a	;playingfx = 1
	pop ix
	pop bc
	ret

;;;
;;FxPlay - En IX ha de venir la dirección de la tabla del FX
;;
PUBLIC FxPlay

FxPlay:
	push ix
	push iy
	push hl
	push bc
	ex af,af'
	push af
	ld a,1
	ld ix, canal_A_fx_estado
	call procesa_fx_canal
	ld a,2
	ld ix, canal_B_fx_estado
	call procesa_fx_canal
	ld a,3
	ld ix, canal_C_fx_estado
	call procesa_fx_canal
	pop af
	ex af,af'
	pop bc
	pop hl
	pop iy
	pop ix
	ret

procesa_fx_canal:
	ex af,af'	;guardo el canal en A'
	ld a,(ix+2)
	and a
	ret z
	ld l,(ix+0)	;puntero actual
	ld h,(ix+1)
	ld a,(ix+6)
	and a
	jr nz,downvolumen
	ld a,(ix+3)	;numero de fines por leer
	and a
	jr nz,do_tick
lee_comandos:
	ld a,(hl)
	inc hl
	ld (ix+0),l
	ld (ix+1),h
	ld b,a		;guardo el valor original
	and %11110000	;formato FGCVxxxx F:ines counter G:oto offset C:oarse V:olumen xxxxx: argumento
	cp %00010000
	jr z,set_fx_volumen
	cp %00100000
	jr z,set_fx_coarse
	cp %00110000
	jr z,set_voldown
	cp %01000000
	jr z,loop_offset
	cp %10000000
	jr z,fines_counter
	xor a	;aquí se llega si se ha puesto el comando $0x -> fin del efecto
	ld (ix+2),a	;playing=0
	ld (ix+5),a	;volumen=0
	dec a
	ld (ix+0),a
	ld (ix+1),a
	jr dumpFx2Aybuffer
set_fx_volumen:
	ld a,b
	and %00001111
	ld (ix+5),a
	jr lee_comandos
set_fx_coarse:		;ojo, después de un coarse siempre viene un finetune
	ld a,b
	and %00001111
	ld (ix+4),a
	ld b,1		;le indico que es 1 finetune inmediato
	jr fines_counter
loop_offset:
	ld a,(hl)
	inc hl
	ld h,(hl)
	ld l,a		;no actualizo el valor en la tabla de IX, ya que lo hace al leer el comando
	jr lee_comandos
set_voldown:
	ld a,b
	and %00001111
	ld (ix+6),a
	jr lee_comandos

downvolumen:
	ld a,(ix+5)
	and a
	jr z,aplica
	dec a
	ld (ix+5),a
aplica:
	ld a,(ix+6)	;downvolumen (numero de ticks bajando)
	dec a
	ld (ix+6),a
	ld a,1
	ld (ix+3),a
	ld a,(ix+7)	;cargo el ultimo finetune del canal
	jr dumpFx2Aybuffer

fines_counter:
	ld a,b
	and %00001111
	ld (ix+3),a	;el puntero ya está actualizado al siguiente elemento, por lo que puedo leer en dotick directamente
do_tick:
	ld a,(hl)	;A= fine_tune
	inc hl
	ld (ix+0),l
	ld (ix+1),h	;actualizo el puntero al siguiente byte tras leer el finetune
	ld (ix+7),a	;el finetune lo guardo
dumpFx2Aybuffer:
	ld iy,Ay_Estado - 2
	ex af,af'	;recupero el numero de canal, y guardo el finetune
	ld b,0
	ld c,a
	add iy,bc
	add iy,bc	;ahora IY apunta al finetune del canal en el buffer de registros del AY
	ex af,af'	;en A' esta el canal, y en A el finetune
	ld (iy+0),a
	ld a,(ix+4)	;pongo el coarse
	ld (iy+1),a
	ex af,af'	;A=Canal
	ld b,0
	ld c,a
	ld iy, Ay_Volumen_A - 1
	add iy,bc	;me posiciono en el volumen
	ld a,(ix+5)
	ld (iy+0),a	;lo pongo en el buffer de registros del AY
actualizaticks:
	ld a,(ix+2)
	and a
	ret z
	ld a,(ix+3)
	dec a
	ld (ix+3),a
	ret

;;;;
;; FxStop detiene el fx que este reproduciendose en el Canal indicado por A (1..3)
;; mutea ese canal instantaneamente en la siguiente actualizacion
FxStop:
  push ix
  push bc
  push af ;guardo el valor del canal
  ld ix,canal_A_fx_estado - size_canalfx_estado
  ld bc, size_canalfx_estado
siguesumando:
  add ix,bc
  dec a
  jr nz, siguesumando
  ld (ix+2),a ;al llegar aquí A vale 0
  pop af ;recupero el valor del canal en C, B queda "sucio"
  ld c,a
  ld ix, Ay_Volumen_A - 1
  ld b,0
  add ix, bc
  ld (ix+0),0 ;pongo a 0 el volumen del canal en el buffer de registros del ay
  pop bc
  pop ix
  ret

FxKillAll:
  push bc
  ld b,3
otrocanal:
  call FxStop
  djnz otrocanal
  pop bc
  ret


;;
;;Si se ha iniciado un Track y aun no ha terminado, se procesan los Canales A y B
;;
PUBLIC MusicPlay

MusicPlay:
	ld a,(playing_mus)
	and a
	ret z
	push iy
	push ix
	push hl
	push de
	push bc
	ld ix, canal_A_estado	;en estas tablas está el estado de cada track (ticks de nota, periodo actual, volumen)
	call procesa_mus_canal
	ld ix, canal_B_estado
	call procesa_mus_canal
	call dumpMus2Ay_buffer
	pop bc
	pop de
	pop hl
	pop ix
	pop iy
	ret

procesa_mus_canal:
	ld a,(ix+2)	;ticks
	and a
	jr nz, procesa_mus_efectos
	ld l,(ix+0)	;cargo en hl el puntero al track
	ld h,(ix+1)
lee_track:
	ld a,(hl)	;;El formato es: ELnnnnnn : E:nd , L:oop  n:numero de nota de la tabla
	ld c,a		;lo guardo en C por si es una nota
	and %11000000	;revisa si es un comando ELnnnnnn : E:nd , L:oop  n:numero de nota de la tabla
	jr z, procesa_nota
	and %10000000	;is end?
	jr z,is_a_loop
	ld (ix+2),0	;lo termino poniendo a 0 el numero de ticks de la nota
	xor a		;dejo playing a 0
	ld (playing_mus),a
	ld (ix+3),a	;volumen a 0
	jr MuteAy

is_a_loop:
	inc hl
	ld a,(hl)
	ld (ix+0),a
	inc hl
	ld h,(hl)
	ld (ix+1),h
	ld l,a		;hl apunta al nuevo offset
	jr lee_track
procesa_nota:
		;en C tengo guardo el valor como nota
	ld b,0	;ahora BC tiene el offset de la nota
	ex de,hl	;guardo el puntero del track en DE
	ld hl,tabla_periodos
	add hl,bc
	add hl,bc	;hl = hl + bc*2 (para leer los 2 bytes del periodo)
	ld a,(hl)
	ld (ix+5),a
	inc hl
	ld a,(hl)
	ld (ix+4),a
	ex de, hl	;recupero el puntero al track
	inc hl		;y ahora leo los ticks (duracion) y el volumen
	ld a,(hl)	;formato TTTTVVVV T:icks , V:olumen
	ld b,a
	and %00001111
	ld (ix+3),a
	ld a,b		;cojo el numero de ticks
	rrca
	rrca
	rrca
	rrca
	and %00001111	;ticks, no lo incremento para que valga: entre 0 y 15, ya que este primer momento ya es un tick de la nota
	ld (ix+2),a
	inc hl	;ahora guardo el puntero del track actualizado
	ld (ix+0),l
	ld (ix+1),h
	ret
procesa_mus_efectos:
	dec a
	ld (ix+2),a
	ld a,(ix+6)	;si no hay que procesar el volumen
	and a
	ret z
	ld a,(ix+3)
	and %00001111	;si el volumen ya es 0 no hace nada
	ret z
	dec a
	ld (ix+3),a
	ret


;esta rutina vuelca segun el estado de los tracks de musica en la tabla Ay
dumpMus2Ay_buffer:
	ld hl,canal_A_estado+4
	ld de,Ay_Canal_A
	ldi
	ldi
	ld hl,canal_B_estado+4
	ldi
	ldi
	ld hl,canal_A_estado+3
	ld de,Ay_Volumen_A
	ldi
	ld hl,canal_B_estado+3
	ldi
	ret
;;;;
;;MusicStop
;;
;; detiene lo que se esté reproduciendo de música
MusicStop:
	xor a
	ld (playing_mus),a
	jr MuteAy

;;;;
;;Mute - Deja las tablas de AY para que se quede muteado. Ojo, no modifica ninguno de los osciladores
;;
;; en la siguiente actualización del Ay, se dejarán de oir los 3 canales
PUBLIC MuteAy

MuteAy:
	push hl
	ld hl,Ay_Mixer
	ld a,%11111000	;Mixer a 0, menos los tonos
	ld (hl),a
	inc hl
	xor a
	ld (hl),a	;los 3 volumenes de canal a 0
	inc hl
	ld (hl),a
	inc hl
	ld (hl),a
	pop hl

;;;;
;;Esta rutina vuelca la Ay table en los registros del AY
;;
;; fuerza a actualizar todos los registros del AY, segun lo que haya en el buffer
PUBLIC UpdateAy

UpdateAy:
	push hl
	push bc
	ld hl,Ay_Estado
	LD a, 0
	ld c,$fd
loop:	LD b,$ff	;FFFD seleccion de registro del AY
	out (c),a
	ld b,$bf	;BFFD envio del dato al registro
	outi
	inc a
	cp 14
	jr nz,loop
	pop bc
	pop hl
	ret


playing_mus: db 0
canal_A_estado:
	dw 0	;+0 offset
	db 0	;+2 ticks_note
	db 0	;+3 volumen
	db 0	;+4 fine_tune
	db 0	;+5 coarse
	db 1	;+6 Si es 1 se baja el volumen en cada tick

canal_B_estado:
	dw 0	;offset
	db 0	;ticks_note
	db 0	;volumen
	db 0	;fine_tune
	db 0	;coarse
	db 0	;En el canal B no se baja el volumen de las notaas


canal_A_fx_estado:
	dw 0	;+0 offset
	db 0	;+2 playing
	db 0	;+3 ;finetunes seguidos a leer
	db 0	;+4 coarse
	db 0	;+5 volumen
	db 0	;+6 downvolumen
	db 0	;+7 finetune


size_canalfx_estado = $ - canal_A_fx_estado

canal_B_fx_estado:
	dw 0	;+0 offset
	db 0	;+2 playing
	db 0	;;finetunes seguidos a leer
	db 0	;coarse
	db 0	;volumen
	db 0	;downvolumen
	db 0	;finetune

canal_C_fx_estado:
	dw 0	;+0 offset
	db 0	;+2 playing
	db 0	;;finetunes seguidos a leer
	db 0	;coarse
	db 0	;volumen
	db 0	;downvolumen
	db 0	;finetune



;Esta es la tabla de los registros a enviar al AY. Importante dejarlos en buen estado en el momento de la inicialización
;lo mismo en el momento del Mute
Ay_Estado:
Ay_Canal_A:
	db 0	;FineTuneCanal A
	db 0	;Coarse Canal A
Ay_Canal_B:
	db 0	;FineTuneCanal B
	db 0	;Coarse Canal B
Ay_Canal_C:
	db 0	;FineTuneCanal C
	db 0	;FineTuneCanal C
Ay_PitchNoise:
	db 0	;noise
Ay_Mixer:
	db %11111000	;11111000	-> esto activa los 3 canales de tono, y desactiva el ruido en los 3 canales y los IO
Ay_Volumen_A:
	db 0	;Canal A Volumen
Ay_Volumen_B:
	db 0	;Canal B
Ay_Volumen_C:
	db 0	;Canal C
AY_EnvelopeFine:
	db 0
Ay_EnvelopeCoarse:
	db 0
Ay_EnvelopeShape:
	db 0

;orden: coarse,fine
tabla_periodos:
	db $f, $ff, $4, $bf, $4, $8d, $4, $0b, $3, $c4, $3, $94, $3, $2a, $2, $fe, $2, $d6, $2, $b2, $2, $8a, $2, $5f, $1, $c6, $1, $2f
	db $1, $20, $1, $0f, $0, $f1, $0, $e3, $0, $ca, $0, $bf, $0, $b5, $0, $ab, $0, $a1, $0, $97, $0, $90, $0, $80, $0, $78, $0, $71
	db $0, $6b, $0, $65, $0, $5f, $0, $5a, $0, $4b, $0, $47

;;;
;;;Musicas
;;;

;Musica de inicio de partida - Track1 para el Canal A
mInitGameA:
	db 1,$fe, 0,$70, 6,$7e, 1,$fe, 0,$70, 6,$7e, 2,$fe, 0,$70, 7,$7e, 2,$fe, 0,$70, 7,$7e
	db 1,$fe, 0,$70, 6,$7e, 1,$fe, 0,$70, 6,$7e, 6,$fe, 8,$fe, 10,$fe, 11,$fe, $80,0
;Track1 para el canal B
mInitGameB:
	db 23,$3f, 0,$30, 32, $3f, 0, $30, 29,$3f, 0,$30, 26,$3f, 0,$30, 32,$3f, 29,$3f, 0,$70, 26,$7f, 0,$70, 24,$3f, 0,$30
	db 33,$3f, 0,$30, 30,$3f, 0,$30, 27,$3f, 0,$30, 33,$3f, 30,$3f, 0,$70, 27,$7f, 0,$70, 23,$3f, 0,$30, 32,$3f, 0,$30
	db 29,$3f, 0,$30, 26,$3f, 0,$30, 32,$3f, 29,$3f, 0,$70, 26,$7f, 0,$70, 25,$3f, 26,$3f, 27,$3f, 0,$30, 27,$3f, 28,$3f
	db 29,$3f, 0,$30, 29,$3f, 30,$3f, 31,$3f, 0,$30, 32,$7f, 16,$70,0

;Música para entre-fases (suena en bucle)- Track2 para el Canal A
mInterfaseA:
	db 5,$7e, 0,$50, 5,$3e, 0,$10, 5,$7e, 0,$50, 5, $3e, 0,$10, 5,$7e, 0,$50, 5,$3e, 0,$10, 8,$3e, 0,0, 9,$3e
	db 0,0, 10,$3e, 0,0, 11,$3e, 0,0, 5,$7e, 0,$50, 5,$3e, 0,$10, 5,$7e, 0,$50, 5,$3e, 0,$10, 5,$7e, 0,$50
	db 5,$3e, 0,$10, 8,$3e, 0,0, 9,$3e, 0,0, 10,$3e, 0,0, 11,$3e, 0,0, 5,$7e, 0,$50, 5,$3e, 0,$10, 5,$7e, 0,$50
	db 5,$3e, 0,$10, 5,$7e, 0,$50, 5,$3e, 0,$10, 8,$3e, 0,0, 9,$3e, 0,0, 10,$3e, 0,0, 11,$3e, 0,0, 12,$7e, 12,$1e, 11,$3e, 0,0, 10,$3e, 0,0, 9,$3e
	db 0,0, 7,$3e, 0,0, 5,$3e, 0,0, 4,$3e, 0,0, 3,$7e, 0,$10, 4,$7e, 0,$10, 5,$7e, 5,$3e, 0,$50
	db 64	;loop
	dw mInterfaseA

;Track2 para el Canal B
mInterfaseB:
	db 16,$1f, 17,$7f, 16,$1f, 17,$7f, 16,$1f, 17,$7f, 14,$1f, 15,$3f, 13,$3f, 17,$3f, 0,$10, 17,$7f, 19,$1f, 20,$ff
	db 0,$70, 16,$1f, 17,$7f, 16, $1f, 17,$7f, 16,$1f, 17,$7f, 14,$1f, 15,$3f, 13,$3f, 17,$3f, 0,$10, 17,$7f
	db 14,$1f, 15,$ff, 0,$70, 16,$1f, 17,$7f, 16,$1f, 17,$7f, 16,$1f, 17,$7f, 14,$1f, 15,$3f
	db 13,$3f, 17,$3f, 0,$10, 17,$7f, 18,$1f, 19,$7f, 20, $1f, 21,$7f, 0,$10, 21, $1f, 22,$bf, 20,$1f, 21,$7f, 18,$1f, 19,$7f
	db 17,$9f, 18,$1f, 19,$bf, 16,$1f, 17,$ff, 0,$50
	db 64	;loop
	dw mInterfaseB

;;;;FX definiciones
;;
;;Formato-
;;	$0x -> termina el efecto, silenciando el canal (el valor de x se desperdicia)
;;	$1x -> define el volumen, el valor de x (se mantiene mientras no se cambie con un comando similar)
;;	$2x -> define el corase (el valor de x), se toma el siguiente byte como finetune
;;	$3x -> define la caida de volumen. Mantiene la nota actual, durante x ticks, bajando el volumen en cada tick
;;	$4x -> goto offset. Se toman los 2 siguientes bytes como la dirección donde estan los nuevos datos del efecto
;;	$8x -> toma la x como el numero de finetune seguidos que debe leer
;;		$84, $40, $20,$30,$10 -> entiende que $40, $20,$30,$10 son finetunes que debe introducir, manteniendo el coarse
;;		Por ejemplo, si queremos codificar, con volumen 5
;;			1,9b
;; 			1,3d
;; 			1,20
;; 			2,46
;;			3,e6
;;
;; 				db $15 (volumen 5)
;; 				db $21, 9b (define el coarse a 1, y ya lee el finetune)
;; 				db $82, $3d, $20 (mantiene el coarse anterior, y lanza como fine tune $3d, y en el siguiente tick $20)
;; 				db $22, $46
;; 				db $23, $e6
;;				db 0	(termina el efecto)
;;
PUBLIC fxInsert_coin
PUBLIC fxComeBola_1
PUBLIC fxComeBola_2
PUBLIC fxComeFruta
PUBLIC fxComeFantasma
PUBLIC fxOjosACasa
PUBLIC fxMuerte
PUBLIC fxTinTin
PUBLIC fxSirena1
PUBLIC fxSirena2
PUBLIC fxSirena3
PUBLIC fxSirena4
PUBLIC fxSirena5
PUBLIC fxHuidaFantasmas

fxInsert_coin:
	db $1f, $21, $02, $82, $3d, $9b, $22, $46, $23, $e6, $2d, $a7, $23, $e6, $22, $46, $21, $9b, $82, $3d, $02, $20, $da, $81, $bc, 0
fxComeBola_1:
	db $1c, $21, $f3, $82, $5d, $0c, $20, $da, $81, $b7, 0
fxComeBola_2:
	db $1c, $20, $a6, $82, $c2, $e9, $21, $23, $81, $84, 0
fxComeFruta:
	db $1f, $20, $9e, $84, $ae, $c2, $da, $f9, $21, $23, $82, $5d, $b4, $22, $46, $23, $69, $26, $d3, $23, $69, $22, $46
	db $21, $b4, $82, $5d, $23, $20, $f9, $86, $da, $c2, $ae, $9e, $91, $86, 0
fxComeFantasma:
	db $1c, $2d, $a7, $26, $d3, $24, $8d, $23, $69, $22, $bb, $81, $46, $21, $f3, $86, $b4, $84, $5d, $3d, $23, $0c, $20, $f9
	db $8f, $e9, $da, $cd, $c2, $b7, $ae, $a6, $9e, $97, $91, $8b, $86, $81, $7c, $78, $82, $74, $70, 0
fxOjosACasa:
	db $1a, $20, $20, $8F, $22, $25, $27, $2a, $2d, $31, $36, $3c, $43, $4b, $57, $66, $7c, $9e, $da
	db $40
	dw fxOjosACasa + 1	;pongo el offset del loop evitando el $1a que es el comando que pone el volumen a "A"
fxMuerte:
	db $1f, $20, $70, $8f, $74, $78, $7c, $81, $7c, $78, $74, $70, $6d, $69, $78, $7c, $81, $86, $8b
	db $1e, $87, $91, $8b, $86, $81, $7c, $78, $74
	db $1d, $8c, $81, $86, $8b, $91, $97, $9e, $97, $91, $8b, $86, $81, $7c
	db $1c, $8c, $8b, $91, $97, $9e, $a6, $ae, $a6, $9e, $97, $91, $8b, $86
	db $1b, $8c, $97, $9e, $a6, $ae, $b7, $c2, $b7, $ae, $a6, $9e, $97, $91
	db $1a, $89, $a6, $ae, $b7, $c2, $cd, $da, $cd, $c2, $b7
	db $18, $21, $b4, $20, $da, $89, $91, $6d, $57, $48, $3e, $36, $30, $2b, $27, $10, $2f, $ff
	db $18, $21, $b4, $20, $da, $89, $91, $6d, $57, $48, $3e, $36, $30, $2b, $27, $10, $2f, $ff, 0

fxTinTin:
	db $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da
	db $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da, $1e,$3c, $20,$da, 0

fxSirena1:
	db $16, $20, $c2, $8f, $ae, $9e, $91, $86, $7c, $74, $6d, $66, $61, $5b, $61, $66, $6d, $74, $7c
	db $87, $86, $91, $9e, $ae, $c2, $da, $f9
	db $40
	dw fxSirena1 + 1

fxSirena2:
	db $16,$20,$9b,$8f, $8b, $7f, $74, $6b, $63, $5d, $57, $52, $4d, $52, $57, $5d, $63, $6b, $74, $85, $7f, $8b, $9b, $ae, $c7
	db $40
	dw fxSirena2 + 1

fxSirena3:
	db $16, $20, $81, $8f, $74, $69, $61, $59, $53, $4d, $48, $44, $48, $4d, $53, $59, $61, $69, $74, $83, $81, $91, $a6
	db $40
	dw fxSirena3 + 1

fxSirena4:
	db $16,$20, $68, $8f, $5e, $56, $4f, $49, $44, $40, $3c, $40, $44, $49, $4f, $56, $5e, $68, $74, $81, $83
	db $40
	dw fxSirena4 + 1

fxSirena5:
	db $16, $20, $57, $8e, $4f, $48, $43, $3e, $3a, $36, $3a, $3e, $43, $48, $4f, $57, $61, $6d
	db $40
	dw fxSirena5 + 1

fxHuidaFantasmas:
	db $1a, $29, $1a, $24, $8d, $23, $08, $22, $46, $21, $d2, $82, $84, $4c
	db $40
	dw fxHuidaFantasmas + 1

current_siren:
    db 1

current_bola:
    db 0
EXTERN _tick
EXTERN _pick

;;;;;;;;;;;;;;;;;;;;;;
; void setup_int(void)
;;;;;;;;;;;;;;;;;;;;;;

SECTION code_user


PUBLIC _setup_int

_setup_int:

   ; im2 table @ 0xd000

   ld hl,0xd000
   ld de,0xd001
   ld bc,256

   ld (hl),0xd1
   ldir

   ; jump to isr

   ld a,0xc3
   ld (0xd1d1),a

   ld hl,isr
   ld (0xd1d2),hl

   ; I register

   ld a,0xd0
   ld i,a

   im 2
   ei

   ret

;;;;;;;;;;;;;;;;;;;
; interrupt routine
;;;;;;;;;;;;;;;;;;;

SECTION code_crt_common  ;; place very low in memory, out of top 16k

PUBLIC isr
PUBLIC isr_skip



isr:

   push af
   push bc
   push de
   push hl
   exx
   ex af,af'
   push af
   push bc
   push de
   push hl
   push ix
   push iy

   ; update clock

   ld a,(_tick)
   inc a
   ld (_tick),a



isr_skip:

   ; music

   ld a,0x80
   ld i,a                      ; point I at uncontended bank

   ld a,h
   or l

   ; FX
   ld a,(_pick)
   or a
   JR Z, it_was_zero  ; skip if it reaches zero

   dec a              ; else decrement
   ld (_pick),a
   call  zap

it_was_zero:
   ld a,0xd0
   ld i,a                      ; restore I

   pop iy
   pop ix
   pop hl
   pop de
   pop bc
   pop af
   ex af,af'
   exx
   pop hl
   pop de
   pop bc
   pop af

   ei
   reti

zap:
	ld d,16		;speaker = bit 4
	ld e,0		;distance between speaker move counter
	ld b, 2   	;overall length counter
blp0:	ld a,d
	and 248		;keep border colour the same
	out (254),a	;move the speaker in or out depending on bit 4
	cpl		;toggle, so we alternative between speaker in and out to make sound
	ld d,a		;store it
	ld c,e		;now a pause
blp1:	dec c
	jr nz,blp1
	inc e		;change to inc e to reverse the sound, or remove to make it a note
	djnz blp0	;repeat B=255 times
	ret
	;

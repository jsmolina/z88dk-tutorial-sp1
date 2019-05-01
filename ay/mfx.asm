; modified for z88dk/z80asm and Gandalf

; mFX Player v0.1
; by Intense in 2017
;
; A sound effect player intended for simultaneous AY music+fx.
; Uses sound effects authored with Shiru's AyFxEdit.
; Uses a single redefinable AY channel for playback.
; If a new sound effect is added while another one is
; playing, the latter is interrupted and discarded.
;
; WARNING: This program expects the stack to contain
; AY register data. It should preferrably be used alongside
; the included mFX edition of the Vortex Tracker II player,
; or another AY music player that supports this mechanism.
; Do not call MFXPLAYM without a preceding call to the music
; player in case you want to play an effect on its own!
; Use the entry point MFXPLAYE for this instead.
;
; Init:
;   call MFXINIT
;
; Adding a new effect:
;   ld hl,EffectAddress  ; MSB = 0 means no effect
;   call MFXADD
;
; Once per frame (Vortex Tracker II example):
;   ld hl,VT2PLAYER+10  ;setup byte in the VTII player (SETUP)
;   set 1,(hl)          ;enable stack output for AY reg data
;   call VT2PLAYER+5    ;process VTII frame, SP=SP-14 at exit (PLAY)
;   call MFXPLAYM       ;process fx and OUTput to AY, SP=SP+14



; AY channel to use for playing back the sound effects
; Values are 0 for channel A, 1 for channel B, 2 for channel C
; Default: 1

defc mfxAyChannel = 2



; Initializes the player.
; Mutes the AY and sets up the variables.
;

MFXINIT:
   xor a
   ld l,a
   ld h,a
   ld (mfxTone+1),hl ;zero tone and noise values
   ld (mfxNoise+3),a
   ld (mfxPtr+2),a      ;set no sound effect
   ld e,14
mfxInit0:
   dec e       ;zero AY channels 0~13
   ld bc,0xFFFD
   out (c),e
   ld b,0xBF
   out (c),a
   jr nz,mfxInit0
   ret



; Sets a new sound effect to play.
; If another effect was playing at the time, it is discarded.
;
; Input: HL = effect address MSB=0 for none

MFXADD:
   ld (mfxPtr+1),hl  ;save it to mfxPtr and exit
   ret



; A secondary entry point used to play back the effects without the music.
;
; Input: None
MFXPLAYE:
   pop de         ;get the return address in DE
   ld hl,0        ;PUSH 14 zero values into the stack
   ld b,7
mfxPlayE0:
   push hl
   djnz mfxPlayE0
   push de


; Main effect playback routine. Must be called each interrupt
; after a call to a mFX compatible music player.
; Expects AY register data (14 bytes) on the stack. At exit,
; SP will increment by 14 not counting the return address.
;
; Input: (SP) = return address followed by AY register data

MFXPLAYM:
   ld ix,2        ;IX = SP+2
   add ix,sp
mfxPtr:
   ld hl,0
   ld a,h
   or a        ;if the effect pointer MSB is 0, that means no sound effect is set
   jr z,MFXOUT    ;in that case, just OUTput the music player AY frame and exit

   ld a,(hl)      ;fetch the frame descriptor byte
   and 0x0F    ;leave only the volume nibble
   ld (ix+8+mfxAyChannel),a;store the volume into the AY buffer
   xor (hl)    ;leave the mixer bits (upper) nibble
   inc hl
   bit 5,a        ;is there a change to the tone pitch?
   jr z,mfxPlayM0    ;if not, skip this part
   ld de,mfxTone+1
   ldi         ;store the new mFX tone pitch
   ldi
mfxPlayM0:
   bit 6,a        ;is there a change to the noise pitch?
   jr z,mfxPlayM1    ;if not, skip this part
   ld de,mfxNoise+3
   bit 5,(hl)     ;is the effect over yet?
   ldi         ;store the new mFX noise pitch
   jr z,mfxPlayM1
   ld h,0         ;if end of effect, set effect ptr MSB to 0 (no playback)
mfxPlayM1:
   ld (mfxPtr+1),hl  ;save the effect pointer
   ld e,a
   or 0x6F-(1<<(mfxAyChannel^3))
mfxPlayM2:
   rrca        ;rotate the mixer bits in the descriptor as needed
   jr c,mfxPlayM2    ;based on the AY channel number
   and (ix+7)     ;combine it with the mixer value from the music player
   ld (ix+7),a    ;save the resulting final mixer value into the AY buffer
   bit 7,e        ;test whether the current mFX effect frame has noise enabled
   jr nz,mfxPlayM3
mfxNoise:
   ld (ix+6),0    ;if it does, overwrite the noise pitch register data from the music player
mfxPlayM3:
   bit 4,e        ;test whether the current mFX effect frame has tone enabled
   jr nz,mfxPlayM4
mfxTone:
   ld hl,0        ;if it does, overwrite the tone pitch register data from the music player
   ld (ix+mfxAyChannel*2),l
   ld (ix+mfxAyChannel*2+1),h
mfxPlayM4:
   xor a

MFXOUT:
   pop ix         ;IX = return address
   ld bc,0xFFFD      ;output the resulting AY register data for this frame and restore SP
   ld de,0xFFBF
   jr mfxOut0
mfxOut1:
   out (c),h
   ld b,d
   inc a
mfxOut0:
   pop hl
   out (c),a
   ld b,e
   out (c),l
   ld b,d
   inc a
   out (c),a
   ld b,e
   cp 13
   jr nz,mfxOut1
   push ix
   bit 7,h        ;the envelope shape register (R13) should only be modified when it has changed
   ret nz         ;this should be indicated by BIT 7 reset in the music player AY data output
   out (c),h
   ret

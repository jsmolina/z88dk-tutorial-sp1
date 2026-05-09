EXTERN _tick
EXTERN _pick
EXTERN _music_playing
EXTERN MusicPlay
EXTERN FxPlay
EXTERN UpdateAy
EXTERN letsplay
EXTERN MuteAy

MAP_SIZE    EQU 800
MAP1_OFFSET EQU 0
MAP2_OFFSET EQU MAP_SIZE
MAP3_OFFSET EQU MAP_SIZE * 2

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

; FX here
  ld a,6
  call enable_bank_n
   call FxPlay
   call MusicPlay
   call UpdateAy
  call restore_bank_0
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

;;;;;;;;;
; banking
;;;;;;;;;

PUBLIC enable_bank_n
PUBLIC enable_bank_6

enable_bank_6:
    ld a, 6
enable_bank_n:

   ; return address

   pop hl

   ; move stack pointer

   ld (temp_sp),sp
   ld sp,temp_stack_top

   ; enable bank

   and 0x07
   or 0x10

   ld bc,0x7ffd
   out (c),a

   ; return

   jp (hl)

; Working variables in low RAM
temp_sp: defw 0

; Safe temporary stack area in low memory (used by enable_bank_n)
temp_stack: defs 16
temp_stack_top:

; currentmap must live below 0xC000 so it is accessible while bank 5 is paged in.
; code_crt_common is placed very low in memory, guaranteeing this.
PUBLIC _currentmap
_currentmap: defs 800, 0

PUBLIC restore_bank_0

restore_bank_0:

   ; return address

   pop hl

   ; restore stack pointer

   ld sp,(temp_sp)

   ; restore bank 0

   ld a,0x10

   ld bc,0x7ffd
   out (c),a

   ; return

   jp (hl)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Map loaders — pure asm, no stack tricks
; Bank 5 paged in/out via direct port I/O
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _load_map1_from_bank

_load_map1_from_bank:
   di
   ld a,0x15              ; bank 5: bit4=1 (ROM select), bits 0-2 = 5
   ld bc,0x7ffd
   out (c),a
   ld hl,0xC000 + MAP1_OFFSET
   ld de,_currentmap
   ld bc,MAP_SIZE
   ldir
   ld a,0x10              ; bank 0
   ld bc,0x7ffd
   out (c),a
   ei
   ret

PUBLIC _load_map2_from_bank

_load_map2_from_bank:
   di
   ld a,0x15
   ld bc,0x7ffd
   out (c),a
   ld hl,0xC000 + MAP2_OFFSET
   ld de,_currentmap
   ld bc,MAP_SIZE
   ldir
   ld a,0x10
   ld bc,0x7ffd
   out (c),a
   ei
   ret

PUBLIC _load_map3_from_bank

_load_map3_from_bank:
   di
   ld a,0x15
   ld bc,0x7ffd
   out (c),a
   ld hl,0xC000 + MAP3_OFFSET
   ld de,_currentmap
   ld bc,MAP_SIZE
   ldir
   ld a,0x10
   ld bc,0x7ffd
   out (c),a
   ei
   ret


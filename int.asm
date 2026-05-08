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

SECTION code_crt_common  ;; place very low in memory, out of top 16k

PUBLIC enable_bank_n
PUBLIC enable_bank_6

enable_bank_6:
    ld a, 6
enable_bank_n:

   ; return address

   pop hl

   ; move stack pointer

   ld (temp_sp),sp
   ld sp,0

   ; enable bank

   and 0x07
   or 0x10

   ld bc,0x7ffd
   out (c),a

   ; return

   jp (hl)

temp_sp: defw 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void load_map_from_bank(uint8_t map_num)
; All 3 maps are stored in BANK_5 at sequential 800-byte offsets:
;   map 1 -> 0xC000 + 0
;   map 2 -> 0xC000 + 800
;   map 3 -> 0xC000 + 1600
; Pages in bank 5, copies 800 bytes from the correct offset
; into _currentmap, then restores bank 0.
; SDCC sdcc_iy calling convention: first uint8_t arg in L.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SECTION code_crt_common

; currentmap must live below 0xC000 so it is accessible while bank 5 is paged in.
; Placing it here alongside the banking routines guarantees that.
PUBLIC _currentmap
_currentmap: defs 800, 0

PUBLIC _load_map_from_bank

_load_map_from_bank:

    ; disable interrupts immediately: ISR uses enable_bank_n which does its
    ; own pop hl / ld sp,0 and would corrupt the stack if it fired here.
    di

    ; L = map_num (1, 2, or 3) via __z88dk_fastcall
    ld a, l
    dec a               ; 0-based: 0, 1, or 2

    ; select source address in HL
    ld hl, 0xC000 + MAP1_OFFSET
    or a
    jr z, lm_page       ; map 1: offset 0
    ld hl, 0xC000 + MAP2_OFFSET
    dec a
    jr z, lm_page       ; map 2: offset 800
    ld hl, 0xC000 + MAP3_OFFSET ; map 3: offset 1600

lm_page:
    ; save SP (return address is still on the stack; move SP away from top 16K)
    ld (temp_sp), sp
    ld sp, 0

    ; page in bank 5
    ld a, 5 | 0x10      ; bank 5, normal ROM, screen 0
    ld bc, 0x7ffd
    out (c), a

    ; copy 800 bytes: HL = bank5 source, DE = _currentmap, BC = 800
    ld de, _currentmap
    ld bc, 800
    ldir

    ; restore bank 0
    ld a, 0x10
    ld bc, 0x7ffd
    out (c), a

    ; restore SP (now points at the return address left by the caller's call)
    ld sp, (temp_sp)
    ei
    ret

;

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

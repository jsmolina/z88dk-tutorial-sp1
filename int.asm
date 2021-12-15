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

; FX here

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

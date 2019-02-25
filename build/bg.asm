SECTION BANK_6


PUBLIC _cartoon0
_cartoon0:
BINARY "intro.scr"


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

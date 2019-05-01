SECTION rodata_user

PUBLIC mfx_add
PUBLIC _mfx_playe
PUBLIC mfx_mfxptr
PUBLIC _FXINIT

defc mfx_init  = _FXINIT
defc mfx_add   = MFXADD
defc _mfx_playe = MFXPLAYE
defc mfx_mfxptr = mfxPtr

INCLUDE "mfx.asm"


PUBLIC _ay_fx_click
PUBLIC _ay_fx_wowow


_ay_fx_click:
  BINARY "fx/click.afx"
_ay_fx_wowow:
  BINARY "fx/wowow.afx"

_FXINIT:
    call MFXINIT
    ld hl, _ay_fx_wowow  ; MSB = 0 means no effect
    call MFXADD
    ret

;-Minimal ayFX player v0.15 06.05.06---------------------------;
;                                                              ;
;  se modifican AF,BC,DE,HL,IX.                    ;
;                                                              ;
; inicialización:                                              ;
;   ld hl, dirección banco SFX                       ;
;   call AFXINIT                                               ;
;                                                              ;
; prepara sfx en buffer:                                       ;
;   ld a, (0..255)     nº de SFX                        ;
;   call AFXPLAY                                               ;
;                                                              ;
; reproduce sfx:                                          ;
;   call AFXFRAME                                              ;
;                                                              ;
;--------------------------------------------------------------;

;--------------------------------------------------------------;
;         hl, dirección banco SFX                      ;
;--------------------------------------------------------------;
PUBLIC afxInit
PUBLIC afxFrame
PUBLIC afxPlay



afxInit:      inc hl
      ld (afxBnkAdr+1),hl      ;direccion banco SFX
      ld hl,afxChDesc      ;buffers
      ld de,$00ff
      ld bc,$03fd

afxInit0:      ld (hl),d
      inc hl
      ld (hl),d
      inc hl
      ld (hl),e
      inc hl
      ld (hl),e
      inc hl
      djnz afxInit0

      ld hl,$ffbf
      ld e,$15

afxInit1:      dec e
      ld b,h
      out (c),e
      ld b,l
      out (c),d
      jr nz,afxInit1
      ld (afxNseMix+1),de
      ret

;--------------------------------------------------------------;
;       reproduce sfx                              ;
;--------------------------------------------------------------;

afxFrame:      ld bc,$03fd
      ld ix,afxChDesc

afxFrame0:   push bc

      ld a,11
      ld h,(ix+1)         ; <11
      cp h
      jr nc,afxFrame7
      ld l,(ix+0)

      ld e,(hl)
      inc hl

      sub b            ;:
      ld d,b            ;(11-3=8, 11-2=9, 11-1=10)

      ld b,$ff
      out (c),a
      ld b,$bf
      ld a,e
      and $0f
      out (c),a

      bit 5,e            ;?
      jr z,afxFrame1

      ld a,3            ;:
      sub d            ;3-3=0, 3-2=1, 3-1=2
      add a,a            ;0*2=0, 1*2=2, 2*2=4

      ld b,$ff
      out (c),a
      ld b,$bf
      ld d,(hl)
      inc hl
      out (c),d
      ld b,$ff
      inc a
      out (c),a
      ld b,$bf
      ld d,(hl)
      inc hl
      out (c),d

afxFrame1:   bit 6,e            ;?
      jr z,afxFrame3

      ld a,(hl)
      sub $20
      jr c,afxFrame2         ; $20
      ld h,a
      ld b,$ff
      ld b,c            ;BC
      jr afxFrame6

afxFrame2:   inc hl
      ld (afxNseMix+1),a

afxFrame3:   pop bc            ;B
      push bc
      inc b            ;TN

      ld a,%01101111         ;TN
afxFrame4:   rrc e
      rrca
      djnz afxFrame4
      ld d,a

      ld bc,afxNseMix+2
      ld a,(bc)
      xor e
      and d
      xor e            ;E D
      ld (bc),a

afxFrame5:   ld c,(ix+2)
      ld b,(ix+3)
      inc bc

afxFrame6:   ld (ix+2),c
      ld (ix+3),b
      ld (ix+0),l
      ld (ix+1),h

afxFrame7:   ld bc,4
      add ix,bc
      pop bc
      djnz afxFrame0

      ld hl,$ffbf
afxNseMix:   ld de,0            ;+1(E)=noise, +2(D)=mixer
      ld a,6
      ld b,h
      out (c),a
      ld b,l
      out (c),e
      inc a
      ld b,h
      out (c),a
      ld b,l
      out (c),d
      ret

;--------------------------------------------------------------;
;   a, (0..255)     nº de SFX                                  ;
;--------------------------------------------------------------;

afxPlay:      ld de,0
      ld h,e
      ld l,a
      add hl,hl
afxBnkAdr:   ld bc,0            ;BC=dirección banco SFX
      add hl,bc
      ld c,(hl)
      inc hl
      ld b,(hl)
      add hl,bc
      push hl

      ld hl,afxChDesc
      ld b,3

afxPlay0:      inc hl
      inc hl
      ld a,(hl)
      inc hl
      cp e
      jr c,afxPlay1
      ld c,a
      ld a,(hl)
      cp d
      jr c,afxPlay1
      ld e,c
      ld d,a
      push hl            ;+3 IX
      pop ix

afxPlay1:      inc hl
      djnz afxPlay0
      pop de

      ld (ix-3),e
      ld (ix-2),d
      ld (ix-1),b
      ld (ix-0),b

      ret

afxChDesc:   defs 12   ;buffers
         ;  4 :
         ; +0 (2) =$00)
         ; +2 (2) NOISE-mixer
         ; ...

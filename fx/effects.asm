
	SECTION	rodata_user

	PUBLIC	_myeffects

	defc	_myeffects = EFFECT_TABLE


;SFX format is quite simple:
;
;defb byte1,byte2
;defb byte1,byte2
;...
;defb $ff

;Where byte 1 and bits 7, 6, 5 & 4 from byte2 are the sound frequency
;and bits 3, 2, 1 & 0 of byte2 is the amplitude.
;Effect finishes when &ff is found.

EFFECT_TABLE:
                defw _EFECTO0

_EFECTO0:
    dw 0xB03F,0x7053,0x6033
    dw 0xF044,0xC040,0xB03E
    dw 0xB044,0x7036,0x605C
    dw 0xF072,0xD04C,0xB039
    dw 0xF083,0xD058,0xC077
    dw 0xF09C,0xC069,0xC08B
    dw 0xF0C4,0xD083,0xC063
    dw 0xF105,0xE0B0,0xC1E4
    dw 0xA0B6,0x7058,0x7041
    defb $FF

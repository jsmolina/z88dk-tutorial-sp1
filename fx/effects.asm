
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
            defb $0E, $1E
			defb  $1D, $1C
			defb $1C, $1A
			defb $1B, $18
			defb $1A, $16
			defb  $00, $00
			defb  $00, $00
			defb  $00, $00
			defb  $1A, $16
			defb  $1B, $18
			defb $1C, $1A
			defb  $0D, $1C
			defb  $0E, $1E
			defb $FF

// motorzx.h

// Tabla de colores para un único valor de papel/tinta
// - TINTA -
// 
// NEGRO	0	00000000
// AZUL		1	00000001
// ROJO		2	00000010
// MAGENTA	3	00000011
// VERDE	4	00000100
// AZULADO	5	00000101
// AMARILLO	6	00000110
// BLANCO	7	00000111
// 
// - PAPEL -
//
// NEGRO	0	00000000
// AZUL		8	00001000
// ROJO		16	00010000
// MAGENTA	24	00011000
// VERDE	32	00100000
// AZULADO	40	00101000
// AMARILLO	48	00110000
// BLANCO	56	00111000

// Funciones:

// ___________________________________________
// Borra la pantalla a un color
// Usar como color la suma de valores de la tabla
// que hay al principio
// ___________________________________________

void cls (int color)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld a, (hl)
		
		ld hl, 22528
		ld de, 22529
		ld bc, 767
		ld (hl), a
		ldir
		ld hl, 16384
		ld de, 16385
		ld bc, 6143
		ld (hl),l
		ldir
	#endasm
}

// ___________________________________________
// Delay, produce una pausa en el programa
// ___________________________________________

void delay (int delay)
{
	#asm
	ld hl,2
	add hl,sp
	ld c, 0
	ld b, 2
	uno:
	ld e, (hl)	;operando base
	ld d, 0
	dos:
	dec de
	ld a,d
	or e
	jp nz,dos
	dec bc
	ld a,b
	or c
	jp nz,uno
	ret
	#endasm
}

// ___________________________________________
// Función para sacar un dato por un puerto
// ___________________________________________

void port_out (int port, int value)
{
	#asm
	ld hl,2
	add hl,sp
	ld a, (hl)
	inc hl
	inc hl
	ld c, (hl)
	inc hl
	ld b, (hl)
	out (c),a
	#endasm
}

// ___________________________________________
// Función para leer un puerto
// ___________________________________________

int __FASTCALL__ port_in (int port)
{
	#asm
        	ld      b, h
        	ld      c, l
        	ld      h, 0
        	in      l, (c)
	#endasm
}


// ___________________________________________
// Espera interrupción y para la CPU
// ___________________________________________

void wait_int (void)
{
	#asm
		halt
	#endasm
}

// ___________________________________________
// Desactiva las interrupciones
// ___________________________________________

void disable_int (void)
{
	#asm
		di
	#endasm
}

// ___________________________________________
// Activa las interrupciones
// ___________________________________________

void enable_int (void)
{
	#asm
		ei
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 16x16 a color
// ___________________________________________

void put_sprite_x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 16x16 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto
		ld a,d
		add a,8
		ld d,a
		.salto		; aquí finaliza la suma de 32 a D
		ld b, 8
		ld c,255
		call draw
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a DE
		ld a,d
		adc a,0
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ret
	
		.draw
		ldi
		ldi
		dec de
		dec de
		inc d	
		djnz draw	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 24x24 a color
// ___________________________________________

void put_sprite_x24 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 24x24 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw2
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto2
		ld a,d
		add a,8
		ld d,a
		.salto2		; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw2
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto3
		ld a,d
		add a,8
		ld d,a
		.salto3		; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw2
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ret
	
		.draw2
		ldi
		ldi
		ldi
		dec de
		dec de
		dec de
		inc d	
		djnz draw2	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 32x32 a color
// ___________________________________________

void put_sprite_x32 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 32x32 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw3
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto4
		ld a,d
		add a,8
		ld d,a
		.salto4		; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw3
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto5
		ld a,d
		add a,8
		ld d,a
		.salto5		; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw3
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto6
		ld a,d
		add a,8
		ld d,a
		.salto6		; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw3
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ldi
		ret
	
		.draw3
		ldi
		ldi
		ldi
		ldi
		dec de
		dec de
		dec de
		dec de
		inc d	
		djnz draw3	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 16x24 a color
// ___________________________________________

void put_sprite_16x24 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 16x24 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw4
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto7
		ld a,d
		add a,8
		ld d,a
		.salto7		; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw4
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto8
		ld a,d
		add a,8
		ld d,a
		.salto8		; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw4
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ret
	
		.draw4
		ldi
		ldi
		dec de
		dec de
		inc d	
		djnz draw4	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 24x32 a color
// ___________________________________________

void put_sprite_24x32 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 24x32 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw5
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto9
		ld a,d
		add a,8
		ld d,a
		.salto9		; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw5
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto10
		ld a,d
		add a,8
		ld d,a
		.salto10	; aquí finaliza la suma de 32 a D
		push de		; guardamos DE
		ld b, 8
		call draw5
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto11
		ld a,d
		add a,8
		ld d,a
		.salto11	; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw5
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		push de		; guardamos DE
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ret
	
		.draw5
		ldi
		ldi
		ldi
		dec de
		dec de
		dec de
		inc d	
		djnz draw5	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 32x16 a color
// ___________________________________________

void put_sprite_32x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 32x16 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw6
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto12
		ld a,d
		add a,8
		ld d,a
		.salto12	; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw6
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		ldi
		ldi
		ldi
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		ld a,d
		adc a,0		; aquí finaliza la suma de 32 a DE
		ld d,a
		ldi		; ponemos los colores de abajo
		ldi
		ldi
		ldi
		ret
	
		.draw6
		ldi
		ldi
		ldi
		ldi
		dec de
		dec de
		dec de
		dec de
		inc d	
		djnz draw6	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 8x16 a color
// ___________________________________________

void put_sprite_8x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 8x16 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw7
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto13
		ld a,d
		add a,8
		ld d,a
		.salto13	; aquí finaliza la suma de 32 a D
		ld b, 8
		call draw7
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a DE (D no es necesario en baja resolución)
		ldi		; ponemos los colores de abajo
		ret
	
		.draw7
		ldi
		dec de
		inc d	
		djnz draw7	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

// ___________________________________________
// Posiciona un Sprite de 16x8 a color
// ___________________________________________

void put_sprite_16x8 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	// -------------------------------------------
	// RUTINA DE IMPRESION DE UN SPRITE 16x8 PIXELS
	// CON ATRIBUTOS EN CUALQUIER POSICION DE CARACTER
	// ENTRADAS:
	// D será la posición del cursor vertical en caracteres
	// E será la posición del cursor horizontal en caracteres
	// HL es la posición de memoria donde tenemos el sprite
	// SALIDAS: se escribe en el mapa de pantalla
	// ADVERTENCIAS: no comprueba límites de pantalla
	// -------------------------------------------
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		.draw8
		ldi
		ldi
		dec de
		dec de
		inc d	
		djnz draw8	; decrementa B y si es cero deja de saltar a draw
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		ldi		; imprimimos los colores de arriba
		ldi
		ret
			
	#endasm
}

// ___________________________________________
// Scroll izquierda
// ___________________________________________

void scroll_izquierda (unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld b, 0
		ld c, (hl)
		push bc			; la altura la metemos en la pila
		inc hl
		inc hl
		ld c, (hl)		; BC = ancho
		dec c			; decrementamos en uno el ancho, ya que la columna de la derecha no se moverá
		inc hl
		inc hl
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		; FIN DE LA RECOGIDA DE LOS DATOS DE ENTRADA
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		; FIN DEL CÁLCULO DE LA DIRECCIÓN DE PANTALLA
		ld l, e		; introducimos en HL el valor de DE+1
		inc l
		ld h,d
		.otra
		push de		; guardamos la posición de la línea a mover
		push de		; guardamos la posición de la línea a mover
		ld a,8
		.scroll
		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		pop de
		inc d
		push de
		ld l, e		; introducimos en HL el valor de DE+1
		inc l
		ld h,d
		dec a
		jr nz, scroll	; iteramos las 8 líneas
		; INICIO DEL SCROLL DE LOS ATRIBUTOS
		pop de
		pop de		; recuperamos el valor horizontal
		push de
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		ld l, e		; introducimos en HL el valor de DE+1
		inc l
		ld h,d
		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		; FIN DEL SCROLL DE LOS ATRIBUTOS
		pop de		; recuperamos la línea a mover
		pop hl		; recuperamos la altura
		dec l
		jr nz, seguir	; iteramos hasta terminar el scroll de toda el área
		ret		; FIN DE LA RUTINA
		.seguir
		push hl		; volvemos a guardar la altura
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, saltox
		ld a,d
		add a,8
		ld d,a
		.saltox		; aquí finaliza la suma de 32 a D
		ld l, e		; introducimos en HL el valor de DE+1
		inc l
		ld h,d
		jr otra
	#endasm
}

// ___________________________________________
// Scroll derecha
// ___________________________________________

void scroll_derecha (unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld b, 0
		ld c, (hl)
		push bc			; la altura la metemos en la pila
		inc hl
		inc hl
		ld c, (hl)		; BC = ancho
		dec c			; decrementamos en uno el ancho, ya que la columna de la izquierda no se moverá
		inc hl
		inc hl
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		; FIN DE LA RECOGIDA DE LOS DATOS DE ENTRADA
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		; FIN DEL CÁLCULO DE LA DIRECCIÓN DE PANTALLA
		ld h,d		; sumamos a DE el ancho
		ld l,e
		add hl,bc
		ld d,h
		ld e,l
		ld l, e		; introducimos en HL el valor de DE-1
		dec l
		ld h,d
		.otra2
		push de		; guardamos la posición de la línea a mover
		push de		; guardamos la posición de la línea a mover
		ld a,8
		.scroll2
		push bc		; guardamos los bytes a mover
		lddr		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		pop de
		inc d
		push de
		ld l, e		; introducimos en HL el valor de DE-1
		dec l
		ld h,d
		dec a
		jr nz, scroll2	; iteramos las 8 líneas
		; INICIO DEL SCROLL DE LOS ATRIBUTOS
		pop de
		pop de		; recuperamos el valor horizontal
		push de
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		ld l, e		; introducimos en HL el valor de DE-1
		dec l
		ld h,d
		push bc		; guardamos los bytes a mover
		lddr		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		; FIN DEL SCROLL DE LOS ATRIBUTOS
		pop de		; recuperamos la línea a mover
		pop hl		; recuperamos la altura
		dec l
		jr nz, seguir2	; iteramos hasta terminar el scroll de toda el área
		ret		; FIN DE LA RUTINA
		.seguir2
		push hl		; volvemos a guardar la altura
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, saltox2
		ld a,d
		add a,8
		ld d,a
		.saltox2	; aquí finaliza la suma de 32 a D
		ld l, e		; introducimos en HL el valor de DE-1
		dec l
		ld h,d
		jr otra2
	#endasm
}

// ___________________________________________
// Scroll arriba
// ___________________________________________

void scroll_arriba (unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld b, 0
		ld c, (hl)
		dec c			; decrementamos en uno la altura, ya que la fila inferior no se moverá
		push bc			; la altura la metemos en la pila
		inc hl
		inc hl
		ld c, (hl)		; BC = ancho
		inc hl
		inc hl
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		; FIN DE LA RECOGIDA DE LOS DATOS DE ENTRADA
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		; FIN DEL CÁLCULO DE LA DIRECCIÓN DE PANTALLA
		.otra3
		ld h,d		; ponemos en HL DE + una línea de caracteres
		ld l,e
		ld a,l		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld l,a
		jr nc, saltohl
		ld a,h
		add a,8
		ld h,a		; aquí finaliza la suma a HL de un línea
		.saltohl
		push de		; guardamos la posición de la línea a mover
		push hl		; guardamos la posición de la línea a mover
		push de		; guardamos la posición de la línea a mover
		push hl		; guardamos la posición de la línea a mover
		ld a,8
		.scroll3
		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		pop hl
		pop de

		inc d
		inc h
		
		push de
		push hl
		dec a
		jr nz, scroll3	; iteramos las 8 líneas
		; INICIO DEL SCROLL DE LOS ATRIBUTOS
		pop hl
		pop de
		pop hl		; recuperamos el valor vertical
		pop de		; recuperamos el valor horizontal
		
		push de
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo

		push hl
		ld a,h		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld h,a          ; ya tenemos h listo, h no hay que cambiarlo

		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		; FIN DEL SCROLL DE LOS ATRIBUTOS
		pop hl		; recuperamos la altura
		pop de		; recuperamos la línea a mover
		pop hl		;recuperamos la altura, pero no la metemos en BC
		dec l
		jr nz, seguir3	; iteramos hasta terminar el scroll de toda el área
		ret		; FIN DE LA RUTINA
		.seguir3
		push hl
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, saltox3
		ld a,d
		add a,8
		ld d,a		; aquí finaliza la suma de 32 a D
		.saltox3
		jr otra3
	#endasm
}

// ___________________________________________
// Scroll abajo
// ___________________________________________

void scroll_abajo (unsigned int x, unsigned int y, unsigned int ancho, unsigned int alto)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld b, 0
		ld c, (hl)
		dec c			; decrementamos en uno la altura, ya que la fila inferior no se moverá
		push bc			; la altura la metemos en la pila
		inc hl
		inc hl
		ld c, (hl)		; BC = ancho
		inc hl
		inc hl
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		; FIN DE LA RECOGIDA DE LOS DATOS DE ENTRADA
		ld a, d		; recuperamos el valor vertical
		pop hl
		add a,l		; le sumamos la altura
		dec a
		push hl
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		pop hl
		add a,l		; le sumamos la altura
		dec a
		push hl
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		; FIN DEL CÁLCULO DE LA DIRECCIÓN DE PANTALLA
		.otra4
		ld h,d		; ponemos en DE HL + una línea de caracteres
		ld l,e
		ld a,l		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a
		jr nc, saltoh2
		ld a,h
		add a,8
		ld d,a		; aquí finaliza la suma a HL de un línea
		.saltoh2
		push de		; guardamos la posición de la línea a mover
		push hl		; guardamos la posición de la línea a mover
		push de		; guardamos la posición de la línea a mover
		push hl		; guardamos la posición de la línea a mover
		ld a,8
		.scroll4
		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		pop hl
		pop de

		inc d
		inc h
		
		push de
		push hl
		dec a
		jr nz, scroll4	; iteramos las 8 líneas
		; INICIO DEL SCROLL DE LOS ATRIBUTOS
		pop hl
		pop de
		pop hl		; recuperamos el valor vertical
		pop de		; recuperamos el valor horizontal
		
		push de
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo

		push hl
		ld a,h		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld h,a          ; ya tenemos h listo, h no hay que cambiarlo

		push bc		; guardamos los bytes a mover
		ldir		; scroll de una línea de pixels
		pop bc		; recuperamos los bytes a mover
		; FIN DEL SCROLL DE LOS ATRIBUTOS
		pop de		; recuperamos la línea a mover
		pop hl
		pop hl		; recuperamos la altura, pero no la metemos en BC
		dec l
		jr nz, seguir4	; iteramos hasta terminar el scroll de toda el área
		ret		; FIN DE LA RUTINA
		.seguir4
		push hl
		
		ld a,e		; decrementamos una línea de caracteres (-32 bytes)
		sub a,32
		ld e,a		; aquí finaliza la resta de 32 a E
		jr nc, saltox4
		ld a,d
		sub a,8
		ld d,a		; aquí finaliza la suma de 32 a D
		.saltox4
		jr otra4
#endasm
}

put_partial1h_sprite_x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw21
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto21
		ld a,d
		add a,8
		ld d,a
		.salto21		; aquí finaliza la suma de 32 a D
		ld b, 8
		ld c,255
		call draw21
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		ldi		; imprimimos los colores de arriba
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a DE
		ld a,d
		adc a,0
		ld d,a
		ldi		; ponemos los colores de abajo
		ret
	
		.draw21
		ldi
		dec de
		inc hl
		inc d	
		djnz draw21	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

put_partial2h_sprite_x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		push de
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw22
		pop de		; recuperamos DE
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a E
		jr nc, salto22
		ld a,d
		add a,8
		ld d,a
		.salto22		; aquí finaliza la suma de 32 a D
		ld b, 8
		ld c,255
		call draw22
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		push de		; guardamos la posición en pantalla
		inc hl		; imprimimos los colores de arriba
		ldi
		inc de
		pop de		; recuperamos la posición de pantalla
		ld a,e		; incrementamos una línea de caracteres (+32 bytes)
		add a,32
		ld e,a		; aquí finaliza la suma de 32 a DE
		ld a,d
		adc a,0
		ld d,a
		inc hl		; ponemos los colores de abajo
		ldi
		ret
	
		.draw22
		inc hl
		ldi
		dec de
		inc d	
		djnz draw22	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

put_partial1v_sprite_x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw23
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		.lele23
		inc hl
		inc hl
		djnz lele23
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		ldi		; imprimimos los colores de arriba
		ldi
		ret
	
		.draw23
		ldi
		ldi
		dec de
		dec de
		inc d	
		djnz draw23	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}

put_partial2v_sprite_x16 (unsigned char *posicion, unsigned int x, unsigned int y)
{
	#asm
		ld hl,2			;pasamos la variable de entrada al acumulador
		add hl,sp
		ld d, (hl)
		inc hl
		inc hl
		ld e, (hl)
		inc hl
		inc hl
		ld a, (hl)
	        inc hl
	        ld h, (hl)
	        ld l, a
		ld a, d		; recuperamos el valor vertical
		and 7		; nos quedamos con la posición en el tercio
		rrca
	        rrca
	        rrca		; rotamos para dejar su valor en múltiplos de 32 (linea)
		and 224		; borramos el resto de bits por si las moscas
		or e		; sumamos el valor horizontal
		ld e, a		; e preparado
		ld a, d
		and 24		; modificamos según el tercio de pantalla
		or 64		; nos posicionamos a partir de 16384 (16384=64+0 en dos bytes)
		ld d, a		; d preparado, ya tenemos la posición en pantalla
		push de		; guardamos DE (la posición de pantalla)
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		.lele24
		inc hl
		inc hl
		djnz lele24
		ld b, 8
		ld c,255	; cargamos C con 255 para no afectar B con LDI
		call draw24
				; Ahora imprimimos los atributos
		pop de		; recuperamos el valor horizontal
		ld a,d		; calculamos el valor de posición en la pantalla
	        rra
	        rra
	        rra		; multiplicamos por 32
	        and 3		; nos quedamos con los tres bits bajos
	        or 88		; apuntamos al comienzo del mapa de atributos
	        ld d,a          ; ya tenemos d listo, e no hay que cambiarlo
		inc hl
		inc hl
		ldi		; imprimimos los colores de abajo
		ldi
		ret
	
		.draw24
		ldi
		ldi
		dec de
		dec de
		inc d	
		djnz draw24	; decrementa B y si es cero deja de saltar a draw
		ret
			
	#endasm
}
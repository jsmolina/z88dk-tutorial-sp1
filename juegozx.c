// juego.c
// Esqueleto de juegos de Radastan para ZX Spectrum
// version 0.1 beta

#include <config.h>
#include <spriteszx.h>
#include <lib/motorzx.h>
#include <lib/sonidoszx.h>

#pragma output STACKPTR=65000

unsigned char mapa [] = {
1,1,1,1,1,1,1,1,1,1,
1,0,0,0,1,0,0,0,0,1,
1,0,0,0,1,0,1,0,0,1,
1,0,0,0,1,0,1,0,0,1,
1,0,0,0,1,0,1,0,0,1,
1,0,0,0,1,0,1,0,2,1,
1,0,0,0,1,0,1,1,1,1,
1,0,2,0,0,0,0,0,2,1,
1,0,0,0,0,0,0,0,0,1,
1,1,1,1,1,1,1,1,1,1,};
unsigned char pantalla [768];

void main (void)
{
	unsigned char x,y,z,frame;
	cls (7); // Borramos la pantalla
	port_out (254,0); // Ponemos el borde de color negro
	frame=0;
	// Dibujamos el mapa
	x=0;
	y=0;
	for (z=0;z<100;++z)
	{
		switch (mapa[z])
		{
			case 0:	put_sprite_x16 (sprite_negro, x, y); frame=0; break;
			case 1: put_sprite_x16 (sprite_cubo, x, y); frame=1; break;
			case 2: put_sprite_x16 (sprite_moneda, x, y); frame=2; break;
		}
		pantalla [x+(y*32)]=frame;
		pantalla [x+1+(y*32)]=frame;
		pantalla [x+((y+1)*32)]=frame;
		pantalla [x+1+((y+1)*32)]=frame;
		++x;
		++x;
		if (x==20)
		{
			x=0;
			++y;
			++y;
		}
	}
	x=2;
	y=2;
	z=0;
	put_sprite_x16 (sprite_prota1, x, y);
	while (1)
	{
		if ((port_in(64510)&1)==0 && y>0 && pantalla[x+((y-1)*32)]==0 && pantalla[x+1+((y-1)*32)]==0) // Q
		{
			put_partial1v_sprite_x16 (sprite_negro,x,y+1);
			z=1;
			--y;
		}
		if ((port_in(65022)&1)==0 && y<22 && pantalla[x+((y+2)*32)]==0 && pantalla[x+1+((y+2)*32)]==0) // A
		{
			put_partial1v_sprite_x16 (sprite_negro,x,y);
			z=1;
			++y;
		}
		if ((port_in(57342)&1)==0 && x<30 && pantalla[x+2+(y*32)]==0 && pantalla[x+2+((y+1)*32)]==0) // P
		{
			put_partial1h_sprite_x16 (sprite_negro,x,y);
			z=1;
			++x;
		}
		if ((port_in(57342)&2)==0 && x>0 && pantalla[x-1+(y*32)]==0 && pantalla[x-1+((y+1)*32)]==0) // O
		{
			put_partial1h_sprite_x16 (sprite_negro,x+1,y);
			z=1;
			--x;
		}
		if (z)
		{
			if (frame==0)
			{
				put_sprite_x16 (sprite_prota1, x, y);
				frame=1;
			}
			else
			{
				put_sprite_x16 (sprite_prota2, x, y);
				frame=0;
			}
			z=0;
			sonido_andar();
		}
		wait_int();
		wait_int();
		wait_int();
		wait_int();
	}

}

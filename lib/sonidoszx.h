// sonidoszx.h

// Sonidicos para hacer que tus juegos sean más molones

void sonido_andar (void)
{
	port_out (254,16);
	port_out (254,16);
	port_out (254,0);
	port_out (254,16);
	port_out (254,16);
	port_out (254,0);
}

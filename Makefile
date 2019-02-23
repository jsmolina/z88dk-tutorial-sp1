compile:
	png2sp1sprite ./build/cat_sprites.png -i sprite_protar -f 32 > ./build/misifu.asm
	zcc +zx -v -startup=31 -DWFRAMES=3 -clib=sdcc_iy -Cz--screen=screen.scr -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o alley -create-app
	echo "Done"

compile:
	png2sp1sprite ./build/cat_sprites.png -i sprite_protar -f 32 > ./build/misifu.asm
	zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o alley
	ls *.bin
	appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
	appmake +zx -b alley_CODE.bin --org 24500 --noloader --blockname code -o code.tap
	appmake +zx -b alley_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
	touch alley.tap
	rm alley.tap
	cat loader.tap screen.tap code.tap bank6.tap > alley.tap

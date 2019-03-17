compile:
	./utils/bas2tap loader.bas -a
	png2udg ./build/tile1.png -a > ./build/tile1.asm
	png2sp1sprite ./build/prota_sprites.png -i sprite_protar -f 16 -s rodata_user > ./build/prota.asm
	zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o alley
	ls *.bin
	appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
	appmake +zx -b alley_CODE.bin --org 24500 --noloader --blockname code -o code.tap
	appmake +zx -b alley_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
	#appmake +zx -b alley_BANK_2.bin --org 49152 --noloader --blockname bank2 -o bank2.tap
	touch alley.tap
	rm alley.tap
	cat loader.tap screen.tap code.tap bank6.tap > alley.tap

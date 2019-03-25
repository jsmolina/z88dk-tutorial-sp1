compile:
	./utils/bas2tap loader.bas -a
	png2udg ./build/pill.png -a > ./build/pill.asm
	png2udg ./build/horizontal.png -a > ./build/horizontal.asm
	png2udg ./build/vertical.png -a > ./build/vertical.asm
	png2udg ./build/corner_bottom_left.png -a > ./build/corner_bottom_left.asm
	png2udg ./build/corner_bottom_right.png -a > ./build/corner_bottom_right.asm
	png2udg ./build/corner_left.png -a > ./build/corner_left.asm
	png2udg ./build/corner_right.png -a > ./build/corner_right.asm
	png2udg ./build/vertileft.png -a > ./build/vertileft.asm
	png2udg ./build/vertiright.png -a > ./build/vertiright.asm
	png2udg ./build/topvertileft.png -a > ./build/topvertileft.asm
	png2udg ./build/topvertiright.png -a > ./build/topvertiright.asm
	png2udg ./build/ghostpill.png -a > ./build/ghostpill.asm
	png2sp1sprite ./build/prota_sprites.png -b -i sprite_protar -f 16 -s rodata_user > ./build/prota.asm
	zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o alley
	ls *.bin
	appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
	appmake +zx -b alley_CODE.bin --org 24500 --noloader --blockname code -o code.tap
	appmake +zx -b alley_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
	#appmake +zx -b alley_BANK_2.bin --org 49152 --noloader --blockname bank2 -o bank2.tap
	touch alley.tap
	rm alley.tap
	cat loader.tap screen.tap code.tap bank6.tap > alley.tap

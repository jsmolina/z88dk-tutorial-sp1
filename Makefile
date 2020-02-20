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
	png2udg ./build/bottomvertileft.png -a > ./build/bottomvertileft.asm
	png2udg ./build/terminal_down.png -a > ./build/terminal_down.asm
	png2udg ./build/terminal_right.png -a > ./build/terminal_right.asm
	png2udg ./build/terminal_left.png -a > ./build/terminal_left.asm
	png2udg ./build/terminal_up.png -a > ./build/terminal_up.asm
	png2udg ./build/union_right.png -a > ./build/union_right.asm
	png2udg ./build/union_left.png -a > ./build/union_left.asm
	png2udg ./build/door.png -a > ./build/door.asm
	png2udg ./build/ghostpill.png -a > ./build/ghostpill.asm
	png2udg ./build/vidas.png -a > ./build/vidas.asm
	png2udg ./font/numbers.png -a > ./font/numbers.asm
	png2udg ./font/abecedary.png -a > ./font/abecedary.asm
	png2sp1sprite ./build/puntos.png -b -i puntos -f 16 -s rodata_user > ./build/puntos.asm
	png2sp1sprite ./build/prota_sprites.png -b -i sprite_protar -f 16 -s rodata_user > ./build/prota.asm
	png2sp1sprite ./build/prota_dead.png -b -i sprite_protar_dead -f 16 -s rodata_user > ./build/prota_dead.asm
	png2sp1sprite ./build/red_ghost.png -b -i red_ghost -f 16 -s rodata_user > ./build/red_ghost.asm
	png2sp1sprite ./build/cerezas.png -b -i cerezas -f 16 -s rodata_user > ./build/cerezas.asm
	png2sp1sprite ./build/letterboxes.png -b -i letterboxes -f 72 -s rodata_user > ./build/letterboxes.asm

	zcc +zx -v -startup=31 -DWFRAMES=3 -clib=sdcc_iy -Cz--screen=intro.scr -SO3 --max-allocs-per-node10000 @zproject.lst -pragma-include:zpragma.inc -o msnampac -create-app
	#zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node10000 @zproject.lst -pragma-include:zpragma.inc -o alley
	#appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
	#appmake +zx -b alley_CODE.bin --org 24500 --noloader --blockname code -o code.tap
	#appmake +zx -b alley_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
	#touch alley.tap
	#rm alley.tap
	#cat loader.tap screen.tap code.tap bank6.tap > alley.tap

deps: 
	@gcc utils/bas2tap.c -o utils/bas2tap
	@wget https://github.com/jsmolina/png2sp1sprite/archive/master.zip
	@unzip -o master.zip
	@echo ""
	@echo "**** ALMOST FINISHED **** "
	@echo "Please run    > cd png2sp1sprite"
	@echo " and          > python setup.py install"
	@rm ./master.zip

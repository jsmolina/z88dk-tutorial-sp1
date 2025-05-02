#!/bin/sh
zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o msnampac
z88dk-appmake +zx -b intro.scr --org 16384 --noloader --blockname screen -o screen.tap
z88dk-appmake +zx -b msnampac_CODE.bin --org 25000 --noloader --blockname code -o code.tap
z88dk-appmake +zx -b msnampac_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
bas2tap -sdisk -a10 loader.bas loader.tap
touch msnampac.tap
rm msnampac.tap
cat loader.tap screen.tap code.tap bank6.tap > msnampac.tap

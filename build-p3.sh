#!/bin/sh
zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o msnampac
z88dk-appmake +zx -b intro.scr --org 16384 --noloader --blockname screen -o screen.tap
z88dk-appmake +zx -b msnampac_CODE.bin --org 25000 --noloader --blockname code -o code.tap
z88dk-appmake +zx -b msnampac_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
z88dk-appmake +zx -b msnampac_BANK_1.bin --org 49152 --noloader --blockname bank1 -o bank1.tap
z88dk-appmake +zx -b msnampac_BANK_3.bin --org 49152 --noloader --blockname bank3 -o bank3.tap
z88dk-appmake +zx -b msnampac_BANK_4.bin --org 49152 --noloader --blockname bank4 -o bank4.tap
bas2tap -sdisk -a10 loader-p3.bas loader-p3.tap
rm -f disk screen code bank6 bank1 bank3 bank4
tapsplit loader-p3.tap
tapsplit screen.tap
tapsplit code.tap
tapsplit bank6.tap
tapsplit bank1.tap
tapsplit bank3.tap
tapsplit bank4.tap
dskform -type edsk -format cpcdata msnampac.dsk
cpmcp -f cpcdata msnampac.dsk disk 0:disk
cpmcp -f cpcdata msnampac.dsk screen 0:screen
cpmcp -f cpcdata msnampac.dsk bank6 0:bank6
cpmcp -f cpcdata msnampac.dsk bank1 0:bank1
cpmcp -f cpcdata msnampac.dsk bank3 0:bank3
cpmcp -f cpcdata msnampac.dsk bank4 0:bank4
cpmcp -f cpcdata msnampac.dsk code 0:code

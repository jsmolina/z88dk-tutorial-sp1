# NOTES

## Needed packages

- cpmtools (distro repo)
- libdsk-tools (distro repo)
- Taptools: https://github.com/jorgegv/taptools
- Bas2tap: https://github.com/speccyorg/bas2tap

## Commands

- Create a standard data +3 disk: `dskform -type edsk -format cpcdata test.dsk` (libdsk-tools)
- List files in a DSK image: `cpmls -f cpcdata test.dsk` (cpmtools)
- Copy a file to a DSK image: `cpmcp -f cpcdata test.dsk NOTES.md 0:NOTES.md` (cpmtools)
- Delete a file from a DSK image: `cpmrm -f cpcdata test.dsk NOTES.md` (cpmtools)
- Convert a BASIC program to a TAP, setting header file name ("DISK"), autostart at line 10: `bas2tap -sDISK -a10 test.bas` (bas2tap)
- Extract all binaries from a TAP file: `tapsplit test.tap` (taptools)

## Conversion of a 128K TAP file with banks to a +3 DSK file

- Extract all the TAP binary blocks with `tapsplit`
- Modify the BASIC loader so that it explicitly loads each bank by name, using the filename for that bank (i.e. no LOAD "" CODE, but LOAD "BANK3" CODE)
- Convert the new BASIC loader to TAP again, remember to set the autostart line
- Convert the BASIC TAP to a binary file named DISK
- Create a blank +3 data disk image
- Copy the BASIC loader into the disk image with name "DISK" (this is critical)
- Copy the binary files (main area and banks) into the disk image with the same names you used on the BASIC loader

If your 128K program does not use bank 7, then that's all, and your DSK should run automatically with the LOADER option of the +3 start menu.

## Loader

- Bank data cannot be loaded directly into address 0xC000, even if you have crrectly selected the bank previously. The reason is that when executing a +3DOS call, it immediately pages in bank #7, which is it uses for disk buffers
- The solution to this to load banks to 0x8000 first, and then move the data from 0x8000 to 0xC000 with a small assembler routine which is included in the BASIC loader
- For this to work, the banks need to be loaded first, and finally load the code with the regular mapping (bank #0 at 0xC000)
- The assembler routine is the following trivial code:
```
	ld bc,16384	; num. bytes to copy
	ld de,49152	; destination address = 0xC000
	ld hl,32768	; source address = 0x8000
	ldir
	ret
```
- The ASM routine is POKE'd to address 25000 with content from some DATA lines. We can reuse this address (25000) because the last time we jump to it, the game will be at that address, and not the transfer routine
- The +3 DISK build script and loader are `build-p3.sh` and `loader-p3.bas`

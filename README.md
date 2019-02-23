# SP1 TUTORIAL

## Considerations before start

* Do you want to make a 48k game or a 128k game?
* Do you know how ZX Spectrum paging works?
* Do you have read about IM2 interruptions? (ISR z88dk)
* Read about `SECTION` and how to define public vars from ASM code `PUBLIC`
* You'll need z88dk, of course, so you could download it from here: http://nightly.z88dk.org/
* Interesting to read as well is sp1 library tutorial: https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_01_GettingStarted.md https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_02_SimpleMaskedSprite.md
* png2sp1sprite.py Quick and dirty png to sprite converter. Right now it only uses 1-bit sprites (no color), and ignores color info. Based on https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_04_BiggerSprites.md

### Drawing on the screen with SP1

Graphics are usually composed of pixel information (all pixels are stored as on or off) and color information that goes 
in 8x8 blocks. You'll see that Attribute clash is present everywhere everytime two sprites with different INK appear 
on the same 8x8 box, and that's why lot of games made heroes to be black and white with transparent background 
(aka mask).

There are two types of entities:
1. User-Defined Graphics (UDGs) (cheap on memory usage). Basically, kind of replacing an 8x8 ascii character by your own 'draw'.
2. Sprites (more memory usage). A computer graphic which may be moved on-screen and otherwise manipulated as a single entity.

Usually, UDGs are more intended for background items (even if they move, but SP1 won't help to move them), but it will help 
to repaint them if a sprite is on top of an UDG.

Sprites are more intended for moving overlapping items, like the game hero, enemies, ...

### sprites 
There are also a lot of ways to draw a sprite, main ones are:
1. with SP1_DRAW_MASK2LB => Do you remember target:renegade? sprites were mostly with a border. But 
as a counterpart, it takes MORE memory (double, for sprite info and for mask info)
2. with SP1_DRAW_XOR1LB => Just an XOR operation for drawing the sprite with the background.

In all cases SP1 will take care for you of the redrawing of background after sprite has passed.


Also, the spectrum has the 'border' and the 'paper', as defined in basic, thus border is only used for
color effects usually.

## First steps 

### A C program:
Write a simple program called `alley.c` with black border and black background
```C
#include <z80.h>
#include <stdlib.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

// this is defining that screen will go from 0 - 32 in width and 0-24 in height
struct sp1_Rect full_screen = {0, 0, 32, 24};

int main()
{
  zx_border(INK_WHITE);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_WHITE | PAPER_WHITE,
                  ' ' );
  sp1_Invalidate(&full_screen);

  sp1_UpdateNow();

  while(1) {

  }
}
```

Now create two files: 

zproject.lst 
``` 
alley.c
```

and zpragma.inc (read z88dk documentation for more info about this). 
The most important is CRT_ORG_CODE, it's where your code is placed in memory.
```
#pragma output CRT_ORG_CODE           = 24500      // org of compile todo this should be higher to allocate sprites
#pragma output REGISTER_SP            = 0xd000     // typical stack location when using sp1

#pragma output CRT_ENABLE_CLOSE       = 0          // don't bother closing files on exit
#pragma output CRT_ENABLE_EIDI        = 0x1        // disable interrupts at start
#pragma output CRT_ON_EXIT            = 0          // jump to 0 on exit

#pragma output CLIB_STDIO_HEAP_SIZE   = 0          // no stdio heap (no files)

#pragma output CLIB_EXIT_STACK_SIZE  = 0         // no atexit() functions

#pragma output CLIB_FOPEN_MAX         = -1         // no FILE* list
#pragma output CLIB_OPEN_MAX          = -1         // no fd table

// CREATE A BLOCK MEMORY ALLOCATOR WITH ONE QUEUE

#pragma output CLIB_BALLOC_TABLE_SIZE = 1

#pragma output CRT_STACK_SIZE = 128
```


You'll need of course to compile it
```bash 
zcc +zx -v -startup=31 -DWFRAMES=3 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 \ 
   @zproject.lst -pragma-include:zpragma.inc -o alley -create-app
```
If you had a loading screen you might be adding: `-Cz--screen=screen.scr `

so boring isn't it?

### Adding a sprite
I would recommend using SevenUP (see other tutorials) 
or an automatic sprite to asm creator like mine:
 * cloning my repo: `$ git clone https://github.com/jsmolina/png2sp1sprite.git`
 * installing png2sp1sprite: `python setup.py install`

Now  
1. Create a `/build` directory.
2. Add a png file (it needs to be size multiple of 8)
3. Execute `png2sp1sprite ./build/cat_sprites.png -i sprite_protar -f 32 > ./build/misifu.asm`
4. That will generate a `misifu.asm` file, that you can now.
5. Create a text file named `binaries.lst` and add in first line just the filename so it can be 
found by compiler: `misifu`.
6. Add a link to binaries.lst in `zproject.lst`: `@build/binaries.lst`
7. Now add this in alley.c:
```C
// brings the built binaries into C
extern uint8_t sprite_protar1[];
extern uint8_t sprite_protar2[];
extern uint8_t sprite_protar3[];
extern uint8_t sprite_protar4[];

// allows to define colours for the sprite
static void initialiseColour(unsigned int count, struct sp1_cs *c)
{
  (void)count;    /* Suppress compiler warning about unused parameter */

  c->attr_mask = SP1_AMASK_INK;
  c->attr      = INK_BLACK;
}

// adds a sprite with MASK (SP1_DRAW_MASK2LB)
struct sp1_ss * add_sprite_misifu() {
  struct sp1_ss * sp;
   sp = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, (int)sprite_protar1, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar2, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar3, 1);
  sp1_AddColSpr(sp, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)sprite_protar4, 1);

  sp1_AddColSpr(sp, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  sp1_IterateSprChar(sp, initialiseColour);

  return sp;
}
```

But I guess you want to see the sprite now, add this in MAIN
```C
int main()
{
  struct sp1_ss* sp = add_sprite_misifu();
```

I'd like to move it move it, ... sure!
```C
  while(1) {
     // sprite, rectangle, offset (animations, use 1 instead of zero if animated), y, x, rotationy, rotationx
     sp1_MoveSprAbs(sp, &full_screen, (void*) 1, 10, 12, 0, 0);
  }
```

# SP1 TUTORIAL

## Building this project
1. Makefile uses png2sp1sprite python scripts, which are available to be installed at: https://github.com/jsmolina/png2sp1sprite
Just clone the repo and do a => python setup.py install 
2. bas2tap is compiled for MacOS, you should first compile it yourself for Linux/Windows/whatever os you are using.
3. Or... just run `make deps`

## Considerations before start
* Do you want to make a 48k game or a 128k game?
* Do you know how ZX Spectrum paging works?
* Do you know what's a sprite mask?
* Do you have read about IM2 interruptions? (ISR z88dk)
* Read about `SECTION` and how to define public vars from ASM code `PUBLIC`
* You'll need z88dk, of course, so you could download it from here: http://nightly.z88dk.org/
* Interesting to read as well is sp1 library tutorial: https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_01_GettingStarted.md https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_02_SimpleMaskedSprite.md
* Interesting demo for z88dk as well 
https://github.com/z88dk/z88dk/tree/f6caa95eba4653ed79e0bd3460c4ebfa56820721/libsrc/_DEVELOPMENT/EXAMPLES/zx/demo_sp1/demo2
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
3. Execute `png2sp1sprite ./build/cat_sprites.png -i sprite_protar -f 16 > ./build/misifu.asm`. 
-f is the frame size, if your sprite has animations. If you don't specify more options the script will automatically 
generate a default mask.
4. That will generate a `misifu.asm` file, that you can now add it.
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
sp1_CreateSpr. 
First param is Left bound of mask (if masked), second says that as it is masked it has 2 BYTES (that is mask,pixels). 
Third param is number_of_tiles + 1. Each tile is 8px, so if it is 24px heigh, third param will equal FOUR. 
Last param is the distance from background (higher is nearer to the 'screen').

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
### Moving a sprite
Now we have the MoveSprAbs it's obvious that we'll need to set X, Y coordinates in vars and modify them from keyboard touches.
Introduced in branch example1: https://github.com/jsmolina/z88dk-tutorial-sp1/tree/example1
`input.h` contains different functions `in_stick_keyboard` or `in_stick_kempston`. So the best is creating a function pointer to them,
allowing the user to choose one using a menu or detecting fire button is pressed in one of the devices.
Then function pointer `joy` can be called directly in game loop:
`in = (joy)(&joy_keys);`
So you can verify which one was pressed using binary checks like `in & IN_STICK_UP`

But it moves so fast, right? It's time for the tricks... 

### Collisions between sprites
As any videogame sometimes there will be a collission. Best way to do them in anyvideogame are 'bubble collisions', which means calculating the absolute difference between x1 and x2, then y1 and y2.

`has_collision` function in this file shows the easyest way to do it:
https://github.com/jsmolina/z88dk-tutorial-sp1/blob/master/logic.c#L266
In my case, it returns a pointer to the 'eaten' ghost, so it's easy then to perform some actions over it (like changing ghost state or moving it home).

### Slowdown games
Bad news, you don't have a RTC here. 

But there are solutions:
* IM2 https://chuntey.wordpress.com/2013/10/02/how-to-write-zx-spectrum-games-chapter-17/
* Floating bus http://sky.relative-path.com/zx/floating_bus.html

IM2 is an interruption that will run 50 times per second. It is useful for controlling a bit the speed of the game.

Setting it is tricky:
```
   im2_init((void *)0xd000); // CRT_ORG = 25124
   memset((void *)0xd000, 0xd1, 257);

   z80_bpoke(0xd1d1, 0xc3);
   z80_wpoke(0xd1d2, (unsigned int)isr);

   intrinsic_ei();
```
Then you define the function this way in z88dk:
```
IM2_DEFINE_ISR(isr)
{
   // update the clock
   ++tick;

}
```
Now you have a tick that is incremented 50 times per second... so slowing it down is just controlling that number
as any other timer.
See this change and a lot of other things in this pull request: https://github.com/jsmolina/z88dk-tutorial-sp1/pull/4
But basically take a look on int.c and the call to wait(); in main loop.

# Tiles
Use this pull request to see what was needed to change to use tiles: https://github.com/jsmolina/z88dk-tutorial-sp1/pull/3

As you know, spectrum has the User-Defined-Graphic (UDG). Splib offers them for backgrounds, and the sky is the limit... 
Generate any 8x8 tile, and just execute png2udg. 
Then copy the var that the python script will generate somewhere in your code:
```
➭ png2udg ./build/tile1.png
const uint8_t tile1[] = {0x10, 0x18, 0x18, 0x3c, 0x7c, 0x7e, 0xfe, 0xff};
```

Or if you prefer to use banking, maybe asm will be easier:
```
➭ png2udg ./build/tile1.png -a

SECTION rodata_user
PUBLIC _tile1
._tile1
defb @00010000, @00011000, @00011000, @00111100, @01111100, @01111110, @11111110, @11111111

```

and of course defining variable accessible in C code
`extern uint8_t tile1[];`

And then when painting level, just use:
``` 
sp1_TileEntry('b', tile1);
sp1_PrintAt(17, 15, INK_BLACK | PAPER_CYAN, 'b');
```
SP1 will take care for you of redrawing. 
Think on a game with pacman and 5 enemies. Everytime you draw the hero sprite, and the 5 enemies,
sp1 invalidates the areas where you draw them. That allows SPLIB to only redraw that areas.
If an area is invalidated twice in one loop, it will only redraw it once.

That's not so useful for scrolling, so if you think on scrolling, then it's maybe 
better to do it yourself.

### .AY music
TODO

## Memory ##
Less lines are better as memory is short. e.g. if you are going to assign four struct properties, don't do it repeating the lines again and again, do it.

```
foo[0].x = 5;
foo[0].y = 6;
foo[1].x = 8;
foo[1].y = 9;
...
```
this one will be better:
```
void assign_foo(uint8_t i, uint8_t x, uint8_t y) {
  foo[i].x = x;
  foo[i].y = y;
}

assign_foo(0, 5, 6);
assign_foo(1, 8, 9);
```

### How to do paging (128k)
To be able to page, normal compiling won't be enough so you'll need to create a custom loader.

This loader will be a BASIC PROGRAM, like this loader.bas:
```
10 BORDER 0: PAPER 0
20 CLEAR VAL "24499"
25 POKE VAL "23739",VAL "111"
30 LOAD ""SCREEN$
40 LOAD ""CODE
45 POKE VAL "23388",VAL "22"
50 OUT VAL "32765",PEEK VAL "23388"
60 LOAD ""CODE
65 POKE VAL "23388",VAL "16"
70 OUT VAL "32765",PEEK VAL "23388"
80 RANDOMIZE USR VAL "24500"

```
1. Skip that Bytes: name message that is printed up on loading the code block overwrite part of the screen: POKE VAL "23739",VAL "111"
2. Load in page 0 both code and screen: `30 LOAD ""SCREEN$` and `40 LOAD ""CODE`
3. By using POKE 23388,16+n'` where 'n' is a number between 0 and 7, you can make the
computer switch in page 'n' of the RAM. You will then be able to use
PEEK and POKE in the normal way to examine and change the page.
4. Actual change of value using port: `OUT VAL "32765",PEEK VAL "23388"
5. Run the game from CRT_ORG_CODE (defined in zpragma.inc): `RANDOMIZE USR VAL "24500"`

And then you'll convert the .bin files to real TAP files, and concat them finally in one unique file:
```
    # linker:
   	zcc +zx -v -m -startup=31 -clib=sdcc_iy -SO3 --max-allocs-per-node200000 @zproject.lst -pragma-include:zpragma.inc -o alley

	appmake +zx -b screen.scr --org 16384 --noloader --blockname screen -o screen.tap
	appmake +zx -b alley_CODE.bin --org 24500 --noloader --blockname code -o code.tap
	appmake +zx -b alley_BANK_6.bin --org 49152 --noloader --blockname bank6 -o bank6.tap
	touch alley.tap
	rm alley.tap
	cat loader.tap screen.tap code.tap bank6.tap > alley.tap

```
1. loader.tap is the previous BASIC code
2. screen.tap is just the conversion of the screen to tap with first appmake
3. code.tap is the actual game code.
4. bank6 is the data/code for 6th bank.

### How to load complex backgrounds and let SP1 manage them
(Copied from z88dk forums)

Yes but I think I would use "sp1_IterateUpdateRect(rect,func)" for this instead.  If you pass it a rectangle that covers the full screen, this function will visit all the "struct sp1_update" in the screen in left to right, top to bottom order.  Each "struct sp1_update" is one character square.  So you could do something like this:


```
unsigned char screen[6144];

unsigned char csx, csy;
unsigned char *csdst;

void copy_screen(struct sp1_update *u)
{
   unsigned char *p;

   // locate character square on screen

   p = zx_cxy2saddr(csx, csy);

   // store graphic location and colour in struct update

   u->tile = (unsigned int)csdst;
   u->colour = *zx_saddr2aaddr(p);

   // copy screen graphics to screen buffer

   for (unsigned char i = 0; i != 8; ++i)
   {
      *csdst++ = *p;
      p += 256;
   }

   // next character square

   if (++csx == 32)
   {
      csx = 0;
      ++csy;
   }
}

...

// copy screen from memory bank
...
memcpy(16384,...);

// create screen in buffer from screen$

csx = csy = 0;
csdst = screen;

sp1_IterateUpdateRect(&fs, copy_screen);   // fs = sp1_Rect covering full screen
```
You need 6144 bytes to store the pixels of the screen$ if your display is full size (here I made an array "screen" but you probably want to control where that is placed).

Instead of storing your screens full size you can compress them.  zx7 is included in z88dk so you can simply do "zx7 -f intro.scr" to compress the screen$ before including it as a binary.  In your program:


```
#include <compress/zx7.h>

dzx7_standard(cartoon0, 16384);  // the original author made this src,dst even though c is normally dst,src

## Text Printing
`Zx_print_int _chr _char _str`
`Zx_print_ink _border for colours`
``` 

### Masks
Masks should have at least one pixel bordering the sprite.
* Misifu mask: https://github.com/jsmolina/speccy-misifu/blob/master/sprites/cat_sprites_mask.png
* Misifu sprite: https://github.com/jsmolina/speccy-misifu/blob/master/sprites/cat_sprites.png
* AND there you have a very simple example of using masks for drawing a circle with animations included:
https://github.com/jsmolina/png2sp1sprite/tree/master/sample

* And Some info about offsets: https://www.z88dk.org/forum/viewtopic.php?id=10277

### Fonts
(Thanks Timmy)
fzx is pretty much incompatible with splib... one of them uses a buffer while the other prints directly on screen.
usual way to go is to override the ascii table with a wider udg (8x multiple width)
```
sp1_TileEntry(33, font1);
   for (i=1; i<46; i++)
   {
        sp1_TileEntry(45+i, font1+i*8);
   }
 ```
see for how we added font:

https://github.com/jsmolina/z88dk-tutorial-sp1/pull/46   
### Credits
Credits to jarlaxe for the graphics.



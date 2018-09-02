## png2sp1sprite.py 
Quick and dirty png to sprite converter. Right now it only uses 1-bit sprites (no color), and ignores color info.
Based on 
https://github.com/z88dk/z88dk/blob/master/doc/ZXSpectrumZSDCCnewlib_SP1_04_BiggerSprites.md


## Usage
png2sp1sprite catinv.png -i sprite_prota

## Using in sp1 (using parts of the tutorial)
* HEADS UP: Tested on zcc 20170112 version
* Just generate the sprite from the PNG: 
```
png2sp1sprite cat.png -i mothership_col > gr_window.asm
``` 

* Then Create a file named juegozx2.c

```C
#pragma output REGISTER_SP = 0xD000

#include <z80.h>
#include <arch/zx.h>
#include <arch/zx/sp1.h>

extern unsigned char mothership_col1[];
extern unsigned char mothership_col2[];
extern unsigned char mothership_col3[];
extern unsigned char mothership_col4[];
extern unsigned char mothership_col5[];
extern unsigned char mothership_col6[];

struct sp1_Rect full_screen = {0, 0, 32, 24};

int main()
{
  struct sp1_ss  *mothership_sprite;
  unsigned char x;

  zx_border(INK_BLACK);

  sp1_Initialize( SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                  INK_BLACK | PAPER_WHITE,
                  'X' );
  sp1_Invalidate(&full_screen);

  mothership_sprite = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3, (int)mothership_col1, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col2, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col3, 0);
  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2,    SP1_TYPE_2BYTE, (int)mothership_col4, 0);

  sp1_AddColSpr(mothership_sprite, SP1_DRAW_MASK2RB,  SP1_TYPE_2BYTE, 0, 0);

  x=0;
  while(1)
  {
    sp1_MoveSprPix(mothership_sprite, &full_screen, 0, x++, 0);

    z80_delay_ms(10);
    sp1_UpdateNow();
  }
}

```
* Modify the third parameter of sp1_CreateSpr is correlated with your sprite heigh.
* Do as many calls to sp1_AddColSpr as the script generated. See that last one is 'special'

And it works!

## What about animated sprites?
Just add the next frame next to current sprite (in x), and then use -f WIDTH of each sprite frame.

e.g. an image of 64 px width will generate TWO frames:
`png2sp1sprite cat_animated.png -i sprite_protar -f 32`

See this thread
https://www.z88dk.org/forum/viewtopic.php?id=10277

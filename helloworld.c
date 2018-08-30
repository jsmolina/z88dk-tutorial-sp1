/* C source start */

#include <stdio.h>
#include <input.h>
#include <arch/zx.h>

#define IN_KEY_SCANCODE_o 0x2df
#define IN_KEY_SCANCODE_p 0x1df
#define IN_KEY_SCANCODE_q 0x1fb
#define IN_KEY_SCANCODE_a 0x1fd
#define IN_KEY_SCANCODE_SPACE 0x017f


int main()
{

  zx_cls(PAPER_WHITE);
  while( 1 ) {

    printf("\x16\x01\x01");



    printf("Scan for q returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_q ));
    printf("Scan for a returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_a ));
    printf("Scan for o returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_o ));
    printf("Scan for p returns 0x%04X\n",   in_key_pressed( IN_KEY_SCANCODE_p ));
    printf("Scan for <sp> returns 0x%04X\n\n", in_key_pressed( IN_KEY_SCANCODE_SPACE ));
  }
}

/* C source end */

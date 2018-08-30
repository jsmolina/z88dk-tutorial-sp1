#!/usr/bin/env bash
#If you wanted to find a function that converts a pixel x,y coordinate to a screen address,
# you would look for something like zx_pxy2saddr
# startup is the kind of screen
 zcc +zx -vn -startup=1 -clib=sdcc_iy helloworld.c -o helloworld -create-app
#!/usr/bin/env python
"""
png2sp1sprite.py

Copyright (C) 2018 Jordi Sesmero
Based on the original idea from Juan J. Martinez
Uses sprite format z88dk SP1: ZXSpectrumZSDCCnewlib_SP1_04_BiggerSprites.md

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

"""
__version__ = "1.0.1"

from argparse import ArgumentParser
from PIL import Image

INK = (255, 255, 255)
PAPER = (205, 0, 0)
MASK = (0, 0, 0)

COLORS = [INK, PAPER, MASK,]


def get_value(rgb):
    if rgb[0] > 0 or rgb[1] > 0 or rgb[2] > 0:
        return "1"
    else:
        return "0"


def binary_formatted(column):
    return '@{}'.format(''.join(column))


def main():

    parser = ArgumentParser(description="png2sprite",
                            epilog="Copyright (C) 2018 Jordi Sesmero",
                            )

    parser.add_argument("--version", action="version", version="%(prog)s "  + __version__)
    parser.add_argument("-i", "--id", dest="id", default="sprite", type=str,
                        help="variable name (default: sprite)")

    parser.add_argument("image", help="image to convert", nargs="?")

    args = parser.parse_args()

    if not args.image:
        parser.error("required parameter: image")

    try:
        image = Image.open(args.image)
    except IOError:
        parser.error("failed to open the image")

    (w, h) = image.size

    if w % 8 or h % 8:
        parser.error("%r size is not multiple of 8" % args.image)

    # so we support both RGB and RGBA images
    data = list(zip(list(image.getdata(0)), list(image.getdata(1)), list(image.getdata(2))))


    bloques = []

    # si son 32px ira de 8 en 8, son 4 bloques
    for bloque in range(0, w, 8):
        row = []
        for y in range(0, h):
            col = []
            mask_col = []
            # vamos al bloque de 8 que toca (ej: 0 al 8, 8 al 16, 16 al 24, 24 al 32)
            for x in range(bloque, bloque + 8):
                pixel = image.getpixel((x, y))
                col.append(get_value(pixel))
                mask_col.append(get_value(pixel))

            # cada fila es mascara, columna
            row.append("defb {}, {}".format(binary_formatted(mask_col), binary_formatted(col)))

        bloques.append(row)

    print("SECTION rodata_user")
    print("; Blocks: ".format(len(bloques)))
    for i in range(0, 7):
        print("defb @11111111, @00000000")

    for i, bloque in enumerate(bloques):
        print("PUBLIC _{}{}".format(args.id, i))
        print("_.{}{}".format(args.id, i))
        for row in bloque:
            print(row)
        print("")
        for i in range(0, 8):
            print("defb @11111111, @00000000")
        print("")


if __name__ == "__main__":
    main()


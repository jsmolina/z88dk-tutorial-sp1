#!/usr/bin/env bash
zcc +zx -vn juegozx.c -o juegozx.bin -lndos -zorg=24200

# Construyendo cinta: juego es lo que saldra al cargar
# Si quieres pantalla de carga renombra loaderzx_con_pantalla.bas por loaderzx.bas
utilszx/bas2tap -a10 -sJUEGO loaderzx.bas loaderzx.tap

# Si quieres pantalla de carga quita el siguiente "rem"
# utilszx/bin2tap -o screenzx.tap -a 16384 loadingzx.bin
utilszx/bin2tap -o mainzx.tap -a 24200 juegozx.bin



cat loaderzx.tap mainzx.tap > juegozx.tap


rm loaderzx.tap
rm mainzx.tap
rm juegozx.bin
rm *.def
alias appmake=z88dk-appmake
zcc +zx -Cz--plus3  -v -m -startup=31 -clib=sdcc_iy  @zproject.lst -Cz--screen=intro.scr -pragma-include:zpragma.inc  -o msnampac -create-app

# Convertir loader a formato tokenizado +3DOS

# Añadir bloques binarios al disco ya creado
# appmake +zx  --screen intro.scr --plus3 -b msnampac --label CODE --auto  -o msnampac.dsk

#!/bin/sh
set -e
set -x
touch msnampac.tap && rm msnampac.tap
mkdir -p /out
/utils/bas2tap -sdisk -a10 loader.bas loader.tap
cat loader.tap screen.tap code.tap bank6.tap > /out/msnampac.tap

echo "DSK GENERATION ###################"
/utils/bas2tap -sdisk -a10 loader-p3.bas loader-p3.tap

rm -f disk screen code bank6 
/utils/taptools/tapsplit loader-p3.tap
/utils/taptools/tapsplit screen.tap
/utils/taptools/tapsplit code.tap
/utils/taptools/tapsplit code.tap
/utils/taptools/tapsplit bank6.tap

dskform -type edsk -format cpcdata /out/msnampac.dsk

/usr/bin/cpmcp -f cpcdata  /out/msnampac.dsk disk 0:disk
/usr/bin/cpmcp -f cpcdata  /out/msnampac.dsk screen 0:screen
/usr/bin/cpmcp -f cpcdata  /out/msnampac.dsk bank6 0:bank6
/usr/bin/cpmcp -f cpcdata  /out/msnampac.dsk code 0:code

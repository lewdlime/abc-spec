#!/bin/sh

if [ $# = 0 ]; then
  echo "Usage: $0 <file>"
  exit 1
fi

# get the latest stable release of abcm2ps
# rename it and make sure it's in your PATH
ABCM2PS="abcm2ps-7.8.14"
# possible issues with the static Linux version?

# also get ps2eps or ps2epsi (recommended)
PS2EPS="ps2eps"
PS2EPSI="ps2epsi"

TMP=$(basename $1 .abc)

if [ $TMP = bang -o $TMP = length -o $TMP = measures_en \
  -o $TMP = riu -o $TMP = scale2 -o $TMP = staffnonote ]
then
  $ABCM2PS -O $TMP.ps Sources/$TMP.abc
else
  $ABCM2PS -O $TMP.ps -c Sources/$TMP.abc
fi

if [ $TMP = whistle ] ; then
  $ABCM2PS -O $TMP.ps -c -F Sources/flute.fmt -T1 Sources/$TMP.abc
fi

if [ $TMP = itachords ] ; then
  $ABCM2PS -O $TMP.ps -c -F Sources/italian Sources/$TMP.abc
fi

echo "Converting to PDF..."

# stange behaviour - symbols are cramped after ps2epsi
# if [ $TMP = "deco" ]
# then
  # $PS2EPS $TMP.ps
# else
  $PS2EPSI $TMP.ps $TMP.eps
# fi

epstopdf $TMP.eps*

echo "Removing .ps .eps* files..."
/bin/rm -f $TMP.ps $TMP.eps*

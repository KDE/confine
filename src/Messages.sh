#! /bin/sh
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg` >>rc.cpp
$XGETTEXT `find . -name \*.cpp -o -name \*.h` -o $podir/confine.pot
rm -f rc.cpp

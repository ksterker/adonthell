#!/bin/sh
# First of all clean up the generated crud
rm -f configure config.log config.cache
rm -f config.status aclocal.m4
# WTF is that??
#mv intl/Makefile.in intl/Makefile.bak
rm -f `find . -name 'Makefile.in'`
#mv intl/Makefile.bak intl/Makefile.in
rm -f `find . -name 'Makefile'`

# Regenerate everything
aclocal -I .
libtoolize --force --copy --ltdl
autoheader
automake -Wno-portability --add-missing --copy --foreign
autoconf 

echo
echo "Now type './configure' to prepare Adonthell for compilation."
echo "Afterwards, 'make' will build Adonthell."
echo

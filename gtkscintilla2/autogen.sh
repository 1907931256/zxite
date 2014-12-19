#!/usr/bin/env sh

touch stamp-h
aclocal
libtoolize --automake
automake --add-missing
autoconf

./configure


Readme for Lua in SciTE

SciTE's version of Lua is copied from the Lua 3.2.2 distribution.

All the .c and .h files from the src and include directories are
copied into SciTE/Lua. The string and io libraries, lstrlib.c liolib.c
from the src/lib directory are also merged into this directory. 
The normal Lua make files are not used. Instead the code is 
compiled and linked directly into SciTE by the make files in the 
win32 and gtk direcories.

Lua is copyright but freely transferrable:
Copyright (c) 1994-1999 TeCGraf, PUC-Rio.  All rights reserved.
See the lua.h file for all the details.

Lua on the web: http://www.tecgraf.puc-rio.br/lua/

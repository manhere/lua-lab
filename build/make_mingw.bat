@echo off
setlocal
cd /d %~dp0

rem for MinGW-TDM 4.5.1

cd ../src

set ALL_FILE_C=lapi.c lauxlib.c lbaselib.c lbitlib.c lbuflib.c lcode.c lcorolib.c lctype.c ldblib.c ldebug.c ldo.c ldump.c lfunc.c lgc.c linit.c liolib.c llex.c lmathlib.c lmem.c loadlib.c lobject.c lopcodes.c loslib.c lparser.c lstate.c lstring.c lstrlib.c ltable.c ltablib.c ltm.c lundump.c lvm.c lzio.c lua.c luac.c

set LIB_FILE_O=lapi.o lauxlib.o lbaselib.o lbitlib.o lbuflib.o lcode.o lcorolib.o lctype.o ldblib.o ldebug.o ldo.o ldump.o lfunc.o lgc.o linit.o liolib.o llex.o lmathlib.o lmem.o loadlib.o lobject.o lopcodes.o loslib.o lparser.o lstate.o lstring.o lstrlib.o ltable.o ltablib.o ltm.o lundump.o lvm.o lzio.o

set COMMON_CP=-O3 -s -pipe -static -ffast-math -fweb -flto

gcc %COMMON_CP% -fwhole-program -c -I../include -DNDEBUG    %ALL_FILE_C%
gcc %COMMON_CP% -fwhole-program -o lua.exe            lua.o %LIB_FILE_O%
gcc %COMMON_CP% -fwhole-program -o luac.exe          luac.o %LIB_FILE_O%
gcc %COMMON_CP% -shared         -o lua.dll ../build/lua.def %LIB_FILE_O%

del *.o

echo Make completed!
pause

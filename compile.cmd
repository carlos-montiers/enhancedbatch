@Echo Off
REM Compiler used: TDM GCC MinGW-w64 edition 5.1.0-2
REM If you need optimize for size add #pragma GCC optimize ("Os") to all c files
REM and use -O3. That will produce less bytes than using -Os.

mingw32-make
mingw32-make clean

:: If you need optimize for size add #pragma GCC optimize ("Os") to all c files
:: and use -O3. That will produce less bytes than using -Os.
REM Compiling using TDM GCC MinGW-w64 edition 5.1.0-2
@mingw32-make %*

@echo off
If Defined WT_SESSION (
  Echo @IMAGE is not supported in Windows Terminal.
  Goto :Eof
)
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
If Not Defined @enhancedbatch Goto :Eof

if %$#%==0 (
  Echo %0 image
  Goto :Eof
)
call @image /q %1
if %errorlevel%==0 (
  Echo Failed to load "%~1".
  Goto :Eof
)
:: Keep it simple for now and assume all images are the same size and delay.
set $width=%errorlevel%
set /a $delay=%$width% ^>^> 16
set /a $height=%$width% ^>^> 8 ^& 0xFF
set /a $width=%$width% ^& 0xFF

:: Make room for the image.
for /l %%L in (2,1,%$height%) do echo.
set /a $r=%@row% - (%$height% - 1)
set @row=%$r%

set $cursor=@cursor
set @cursor=off
set $delayedexpansion=@delayedexpansion
set @delayedexpansion=on

call @image /n %1
set $frames=%errorlevel%
if %$delay%==0 set $delay=100
if %$frames%==1 (
  call @image %1
  call @getkb
) else (
  set $f=1
  for do (
    call @sleep %$delay%
    :: Need a better way to handle transparent animation.
    call @clear /c %$NBSP% %@position% %$height% %$width%
    call @image /f !$f! %1
    set /a $f=(!$f!+1^) %% %$frames%
    call @kbhit || set @next=
  )
)
:: Legacy console will not necessarily erase spaces, so clear with no-break
:: space to ensure the window will be redrawn.
call @clear /c %$NBSP% %@position% %$height% %$width%
call @clear %@position% %$height% %$width%

set @delayedexpansion=$delayedexpansion
set @cursor=$cursor

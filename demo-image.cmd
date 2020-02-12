@echo off
If Defined WT_SESSION (
  Echo @IMAGE is not supported in Windows Terminal.
  Goto :Eof
)
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
If Not Defined @enhancedbatch Goto :Eof

set "$demoimgfile=batch-hero.gif"

if "%~1"=="/?" (
  call @image /?
  goto :eof
)
if /i "%~1"=="/D" (
  set $fixedDelay=%2
  shift /1
  shift /1
)
set "$imgfile=%~1"
set "$options=%$2-%"
if not defined $imgfile (
  set "$imgfile=%$0;~dp%%$demoimgfile%"
  Echo Usage: %0 [/D delay] image [options]
  Echo Using "%$demoimgfile%" as example.
)
call @image %$options% /q "%$imgfile%"
if %errorlevel%==0 (
  Echo Failed to load "%$imgfile%".
  Goto :Eof
)
:: Keep it simple for now and assume all images are the same size.
set $size=%errorlevel%
set /a $height=%$size% ^>^> 8 ^& 0xFF
set /a $width=%$size% ^& 0xFF

set $delayedexpansion := @delayedexpansion
set @delayedexpansion=on

for /l %%O in (2,1,%$#%) do (
  if /i "!$%%O!"=="/P" (
    set $r=1
  ) else if /i "!$%%O!"=="/R" (
    set $r=1
  ) else if /i "!$%%O!"=="/RH" (
    set $r=1
  ) else if /i "!$%%O!"=="/F" (
    set $frames=1
  )
)

:: Make room for the image (unless row was given).
if not defined $r (
  for /l %%L in (2,1,%$height%) do echo.
  set /a $r=!@row! - (%$height% - 1^)
  set @row=!$r!
)

set $cursor := @cursor
set @cursor=off
:: Give the cursor a chance to hide before copying the background.
call @sleep 40

call @image %$options% /copy /n "%$imgfile%"
if not defined $frames set $frames=%errorlevel%
if %$frames%==1 (
  call @image %$options% "%$imgfile%"
  call @getkb
) else (
  set $f=0
  for (!@kbhit!==-1) do (
    call @image %$options% /f !$f! "%$imgfile%"
    if defined $fixedDelay (
      call @sleep %$fixedDelay%
    ) else (
      set /a $delay=!errorlevel! ^>^> 16
      if !$delay!==0 set $delay=100
      call @sleep !$delay!
    )
    set /a $f=(!$f!+1^) %% %$frames%
  )
)
call @image %$options% /restore "%$imgfile%"

set @delayedexpansion=%$delayedexpansion%
set @cursor=%$cursor%

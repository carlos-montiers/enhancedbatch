@echo off
If Defined WT_SESSION (
  Echo @TEXT is not supported in Windows Terminal.
  Goto :Eof
)
rundll32 "%~dp0enhancedbatch_%processor_architecture%" load
If Not Defined @enhancedbatch (
  Echo Load Failed
  Goto :Eof
)

:: Legacy console will not necessarily erase spaces, so clear with no-break
:: space to ensure the entire window will be redrawn.
call @clear /c %$NBSP%

call @write /con /p 8 16 /af0 + /c 48 +

:: 10 will redraw the screen between the above and below characters, so having
:: the below after the first set of @TEXT statements will erase some of that.
call @write /con /p 12 24 /af0 + /c 56 +

call @write /con /p 18 35 /af0 +

:: 10 needs a bit of a delay to refresh the screen.
call @sleep 40

call @text /p 8 17 /bg 2 /d default
call @text /p 7 16 /bg 4 /d /rl left
call @text /p 9 16 /bg 5 /d /rr right
call @text /p 8 15 /bg 6 /d /rt twice
call @text /p 8 49 /bg 2 /d /v vertical
call @text /p 7 48 /bg 4 /d /v /rl left
call @text /p 9 48 /bg 5 /d /v /rr right
call @text /p 8 47 /bg 6 /d /v /rt twice

call @text /p 12 25 /fg 2 /d /t default
call @text /p 11 24 /fg 4 /d /t /rl left
call @text /p 13 24 /fg 5 /d /t /rr right
call @text /p 12 23 /fg 6 /d /t /rt twice
call @text /p 12 57 /fg 2 /d /t /v vertical
call @text /p 11 56 /fg 4 /d /t /v /rl left
call @text /p 13 56 /fg 5 /d /t /v /rr right
call @text /p 12 55 /fg 6 /d /t /v /rt twice

set $delayedexpansion=@delayedexpansion
set @delayedexpansion=on
set $fg=0
for /l %%D in (225,225,3600) do (
	set $i=!$fg!
	if !$i!==%@bg% set /a $i=%@bg% ^^ 8
	call @text /e %%D /o %%D /t /ph 18 35 /fg !$i! circle
	set /a $fg=!$fg!+1
)
set @delayedexpansion=$delayedexpansion

set @row=24
pause
call @clear

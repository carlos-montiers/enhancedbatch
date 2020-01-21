:: Run a command using Enhanced Batch, or load/unload it.
@echo off

:: No arguments, load into a batch file.
if "%~1"=="" (
  if not defined @enhancedbatch rundll32.exe "%~dp0enhancedbatch_%processor_architecture%.dll" load
  if not defined @enhancedbatch >&2 echo Enhanced Batch failed to load.&exit /b 1
  goto :eof
)

:: Load into the command line (or a batch, if you don't want the above message,
:: but you won't get the exit code, either).
if "%~1"=="load" (
  if defined @enhancedbatch goto :eof
  2>nul goto&rundll32.exe "%~dp0enhancedbatch_%processor_architecture%.dll" load
)

:: Unload from the command line (DON'T call from a batch).
if "%~1"=="unload" (
  if not defined @enhancedbatch goto :eof
  :: Not really necessary, but it's nice symmetry.
  2>nul goto&call @unload
)

if not defined @enhancedbatch (
  rundll32.exe "%~dp0enhancedbatch_%processor_architecture%.dll" load
  if not defined @enhancedbatch exit /b 1
)
call %*

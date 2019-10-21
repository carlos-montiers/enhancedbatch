:: Run a command using Enhanced Batch.
@echo off
rundll32.exe "%~dp0enhancedbatch_%processor_architecture%.dll" load
if not defined @enhancedbatch echo Enhanced Batch didn't load.&exit /b 1
%*

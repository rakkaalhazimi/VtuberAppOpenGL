@echo off

:: Configure build system
cmake -DMAIN_SRC="%MAIN%" -G "MinGW Makefiles" .

:: Build the project
cmake --build .

if %ERRORLEVEL% NEQ 0 (
  echo Build failed! Exiting...
  exit /b %ERRORLEVEL%
) 

:: Run executable
.\bin\app.exe
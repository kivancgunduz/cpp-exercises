@echo off
REM Usage (from the cpp-exercises folder):  run.bat phase1_basics\ex1_1_stats.cpp
if "%~1"=="" goto usage

where g++ >nul 2>nul
if not errorlevel 1 goto gcc
where cl >nul 2>nul
if not errorlevel 1 goto msvc
goto nocompiler

:gcc
REM MinGW g++ has no AddressSanitizer on Windows, so sanitizers are off here.
g++ -std=c++17 -O1 -g -Wall -Wextra "%~1" -o %TEMP%\app.exe && %TEMP%\app.exe %2 %3
goto :eof

:msvc
REM Visual Studio compiler (run from "Developer Command Prompt for VS"). AddressSanitizer is on.
cl /nologo /std:c++17 /EHsc /W4 /Zi /fsanitize=address "%~1" /Fe%TEMP%\app.exe /Fo%TEMP%\ /Fd%TEMP%\ && %TEMP%\app.exe %2 %3
goto :eof

:nocompiler
echo No C++ compiler found. Install one of: WSL + g++, MSYS2 (MinGW g++), or Visual Studio Build Tools.
exit /b 1

:usage
echo Usage: run.bat phase1_basics\ex1_1_stats.cpp
exit /b 1

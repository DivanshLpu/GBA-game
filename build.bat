@echo off
title MyGBA Build System

set PROJECT=D:\devkitPro\projects\mygba
set MGBA=C:\Program Files\mGBA\mGBA.exe

echo.
echo ==========================
echo Building MyGBA Project...
echo ==========================
echo.

cd /d "%PROJECT%"

D:\devkitPro\msys2\usr\bin\bash.exe -lc "cd /d/devkitPro/projects/mygba && make clean && make"

if errorlevel 1 (
    echo.
    echo ==========================
    echo BUILD FAILED
    echo ==========================
    pause
    exit /b 1
)

echo.
echo ==========================
echo BUILD SUCCESSFUL
echo ==========================
echo.

for %%f in (*.gba) do (
    echo Found ROM: %%f
    start "" "%MGBA%" "%%f"
    goto done
)

echo No .gba file found!

:done
pause
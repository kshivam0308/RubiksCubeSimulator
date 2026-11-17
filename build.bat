@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo   3D Rubik's Cube Simulator - Build Tool
echo ========================================================
echo.

set COMPILER=
g++ --version >nul 2>&1
if !ERRORLEVEL! equ 0 (
    set "COMPILER=g++"
    echo [OK] Using system MinGW g++ compiler from PATH.
) else (
    if exist "%~dp0w64devkit\bin\g++.exe" (
        set "COMPILER=%~dp0w64devkit\bin\g++.exe"
        echo [OK] Using portable 64-bit GCC compiler from w64devkit.
    )
)

if "!COMPILER!"=="" (
    echo [ERROR] No working C++ compiler found!
    pause
    exit /b 1
)

echo.
echo [1/2] Setting up FreeGLUT runtime...
copy /y "%~dp0lib\x64\freeglut.dll" "%~dp0freeglut.dll" >nul 2>&1

echo.
echo [2/2] Compiling C++ source files...
cd /d "%~dp0"
"!COMPILER!" -O2 -Wall src\main.cpp src\Cubie.cpp src\RubiksCube.cpp src\Camera.cpp src\Renderer.cpp src\InputHandler.cpp -Iinclude -Llib\x64 -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32 -static-libgcc -static-libstdc++ -o RubiksCube.exe

set BUILD_RES=!ERRORLEVEL!
if !BUILD_RES! equ 0 (
    echo.
    echo ========================================================
    echo   BUILD SUCCESSFUL: RubiksCube.exe generated!
    echo ========================================================
    echo To start the simulator, run run.bat or RubiksCube.exe
    echo ========================================================
) else (
    echo.
    echo [ERROR] Compilation failed with error code !BUILD_RES!.
)

echo.
endlocal

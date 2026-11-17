@echo off
cd /d "%~dp0"

if not exist RubiksCube.exe (
    echo [INFO] Executable not found. Compiling first...
    call build.bat
    if not exist RubiksCube.exe exit /b 1
)

echo Starting 3D Rubik's Cube Simulator...
start "" "%~dp0RubiksCube.exe"

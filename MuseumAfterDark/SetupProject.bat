@echo off
REM Setup script for MuseumAfterDark Unreal Engine Project
REM This script regenerates Visual Studio project files for C++ compilation

echo ========================================
echo MuseumAfterDark Project Setup
echo ========================================
echo.

REM Check if .uproject file exists
if not exist "MuseumAfterDark.uproject" (
    echo ERROR: MuseumAfterDark.uproject not found!
    echo Please run this script from the MuseumAfterDark folder.
    pause
    exit /b 1
)

echo Step 1: Finding Unreal Engine installation...
echo.

REM Try to find Unreal Engine installation
REM Common installation paths
set "UE_PATH="
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool.exe" (
    set "UE_PATH=C:\Program Files\Epic Games\UE_5.6"
    goto :found_ue
)
if exist "C:\Program Files (x86)\Epic Games\UE_5.6\Engine\Binaries\DotNET\UnrealBuildTool.exe" (
    set "UE_PATH=C:\Program Files (x86)\Epic Games\UE_5.6"
    goto :found_ue
)

REM Try to find any UE installation via registry or common locations
for /d %%i in ("C:\Program Files\Epic Games\UE_*") do (
    if exist "%%i\Engine\Binaries\DotNET\UnrealBuildTool.exe" (
        set "UE_PATH=%%i"
        goto :found_ue
    )
)

echo WARNING: Could not automatically find Unreal Engine installation.
echo Please ensure Unreal Engine 5.6 is installed.
echo.
echo Attempting to use UnrealVersionSelector...
goto :use_selector

:found_ue
echo Found Unreal Engine at: %UE_PATH%
echo.

:use_selector
echo Step 2: Regenerating Visual Studio project files...
echo.

REM Use UnrealVersionSelector to regenerate project files
REM This is the standard way to regenerate project files on Windows
"%ProgramFiles%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" /projectfiles "%CD%\MuseumAfterDark.uproject"

if errorlevel 1 (
    echo.
    echo ERROR: Failed to regenerate project files!
    echo.
    echo Alternative method: Right-click MuseumAfterDark.uproject
    echo and select "Generate Visual Studio project files"
    pause
    exit /b 1
)

echo.
echo ========================================
echo Setup Complete!
echo ========================================
echo.
echo Next steps:
echo 1. Open MuseumAfterDark.uproject in Unreal Engine
echo 2. When prompted, click "Yes" to rebuild modules
echo 3. Wait for compilation to complete
echo.
echo OR
echo.
echo 1. Open MuseumAfterDark.sln in Visual Studio
echo 2. Build the solution (F7)
echo 3. Open MuseumAfterDark.uproject in Unreal Engine
echo.
pause


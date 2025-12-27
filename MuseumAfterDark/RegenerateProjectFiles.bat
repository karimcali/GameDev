@echo off
REM Simple script to regenerate Unreal Engine project files
REM Right-click this file and select "Run as administrator" if needed

echo Regenerating Visual Studio project files for MuseumAfterDark...
echo.

REM Method 1: Use UnrealVersionSelector (most reliable)
if exist "%ProgramFiles%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" (
    "%ProgramFiles%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" /projectfiles "%~dp0MuseumAfterDark.uproject"
    if errorlevel 1 (
        echo Failed to regenerate using UnrealVersionSelector.
        goto :manual
    ) else (
        echo Project files regenerated successfully!
        echo You can now open MuseumAfterDark.uproject
        pause
        exit /b 0
    )
)

:manual
echo.
echo Could not find UnrealVersionSelector.
echo.
echo Please manually regenerate project files:
echo 1. Right-click on MuseumAfterDark.uproject
echo 2. Select "Generate Visual Studio project files"
echo.
pause


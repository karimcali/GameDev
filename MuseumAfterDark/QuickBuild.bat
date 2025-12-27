@echo off
REM Quick build script - builds the project directly without generating project files first
REM Use this if project files are already generated

if not exist "MuseumAfterDark.uproject" (
    echo ERROR: MuseumAfterDark.uproject not found!
    pause
    exit /b 1
)

REM Find UE installation
set "UE_PATH="
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" (
    set "UE_PATH=C:\Program Files\Epic Games\UE_5.6"
) else if exist "C:\Program Files (x86)\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" (
    set "UE_PATH=C:\Program Files (x86)\Epic Games\UE_5.6"
) else (
    for /d %%i in ("C:\Program Files\Epic Games\UE_*") do (
        if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
            set "UE_PATH=%%i"
            goto :build
        )
    )
    echo ERROR: Unreal Engine not found!
    pause
    exit /b 1
)

:build
echo Building MuseumAfterDark project...
echo This may take several minutes...
echo.

"%UE_PATH%\Engine\Build\BatchFiles\Build.bat" MuseumAfterDarkEditor Win64 Development -Project="%CD%\MuseumAfterDark.uproject" -WaitMutex -FromMsBuild

if errorlevel 1 (
    echo Build failed! Check errors above.
    pause
    exit /b 1
)

echo Build completed successfully!
pause


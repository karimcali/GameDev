@echo off
REM Comprehensive build script for MuseumAfterDark Unreal Engine Project
REM This script generates project files AND builds the C++ modules

echo ========================================
echo MuseumAfterDark Project Build Script
echo ========================================
echo.

REM Check if .uproject file exists
if not exist "MuseumAfterDark.uproject" (
    echo ERROR: MuseumAfterDark.uproject not found!
    echo Please run this script from the MuseumAfterDark folder.
    pause
    exit /b 1
)

REM Fix read-only permissions (common issue when extracting from zip)
echo Fixing file permissions (removing read-only attributes)...
attrib -R /S /D *.* >nul 2>&1

REM Delete Intermediate\ProjectFiles folder if it exists (often has read-only files from zip)
if exist "Intermediate\ProjectFiles" (
    echo Cleaning up Intermediate\ProjectFiles folder...
    rd /s /q "Intermediate\ProjectFiles" >nul 2>&1
)

REM Also fix permissions on Intermediate folder specifically
if exist "Intermediate" (
    attrib -R /S /D Intermediate\*.* >nul 2>&1
)
echo.

REM Step 1: Find Unreal Engine installation
echo Step 1: Finding Unreal Engine installation...
set "UE_PATH="

REM Check common installation paths
if exist "C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" (
    set "UE_PATH=C:\Program Files\Epic Games\UE_5.6"
    goto :found_ue
)
if exist "C:\Program Files (x86)\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" (
    set "UE_PATH=C:\Program Files (x86)\Epic Games\UE_5.6"
    goto :found_ue
)

REM Try to find any UE installation
for /d %%i in ("C:\Program Files\Epic Games\UE_*") do (
    if exist "%%i\Engine\Build\BatchFiles\Build.bat" (
        set "UE_PATH=%%i"
        goto :found_ue
    )
)

echo ERROR: Could not find Unreal Engine installation!
echo Please ensure Unreal Engine 5.6 is installed.
pause
exit /b 1

:found_ue
echo Found Unreal Engine at: %UE_PATH%
echo.

REM Step 2: Generate Visual Studio project files
echo Step 2: Generating Visual Studio project files...
echo.

REM Try UnrealVersionSelector first
if exist "%ProgramFiles%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" (
    "%ProgramFiles%\Epic Games\Launcher\Engine\Binaries\Win64\UnrealVersionSelector.exe" /projectfiles "%CD%\MuseumAfterDark.uproject"
    if errorlevel 1 (
        echo WARNING: UnrealVersionSelector failed, trying alternative method...
    ) else (
        echo Project files generated successfully!
        goto :build_project
    )
)

REM Alternative: Use UnrealBuildTool directly to generate project files
echo Using UnrealBuildTool to generate project files...
"%UE_PATH%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%CD%\MuseumAfterDark.uproject" -game -rocket -progress

if errorlevel 1 (
    echo ERROR: Failed to generate project files!
    echo.
    echo Please try manually:
    echo 1. Right-click MuseumAfterDark.uproject
    echo 2. Select "Generate Visual Studio project files"
    pause
    exit /b 1
)

:build_project
echo.
echo Step 3: Building the project...
echo This may take several minutes. Please wait...
echo.

REM Build the project using UnrealBuildTool
"%UE_PATH%\Engine\Build\BatchFiles\Build.bat" MuseumAfterDarkEditor Win64 Development -Project="%CD%\MuseumAfterDark.uproject" -WaitMutex -FromMsBuild

if errorlevel 1 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    echo.
    echo Please check the error messages above.
    echo Common issues:
    echo - Missing Visual Studio C++ tools
    echo - Missing Windows SDK
    echo - Compilation errors in source code
    echo.
    echo You can also try building from Visual Studio:
    echo 1. Open MuseumAfterDark.sln
    echo 2. Set configuration to "Development Editor"
    echo 3. Build the solution (F7)
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo You can now open MuseumAfterDark.uproject in Unreal Engine.
echo.
pause


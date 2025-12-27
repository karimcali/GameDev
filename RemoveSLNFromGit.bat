@echo off
REM This script removes the .sln file from Git tracking
REM Run this ONCE if your .sln file is currently being tracked by Git
REM After running this, the .sln file will be ignored by Git (as per .gitignore)

echo Removing MuseumAfterDark.sln from Git tracking...
echo.

cd MuseumAfterDark

if not exist "MuseumAfterDark.sln" (
    echo MuseumAfterDark.sln not found in current directory.
    pause
    exit /b 1
)

git rm --cached MuseumAfterDark.sln

if errorlevel 1 (
    echo.
    echo WARNING: Git command failed. This might mean:
    echo - You're not in a Git repository
    echo - The file is not tracked by Git
    echo - Git is not installed or not in PATH
    echo.
    echo If the file is not tracked, you can ignore this message.
) else (
    echo.
    echo Successfully removed MuseumAfterDark.sln from Git tracking.
    echo The file will remain on your disk but won't be committed anymore.
    echo.
    echo Next step: Commit this change with:
    echo   git commit -m "Remove .sln from Git tracking (should be regenerated)"
)

echo.
pause


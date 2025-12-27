@echo off
REM Fix read-only permissions on project files
REM This is needed when extracting from zip files

echo Fixing file permissions...
echo.

REM Remove read-only attribute from all files and folders
attrib -R /S /D *.*

if errorlevel 1 (
    echo WARNING: Some files may still be read-only.
    echo Try running this script as Administrator.
) else (
    echo File permissions fixed successfully!
)

echo.
pause


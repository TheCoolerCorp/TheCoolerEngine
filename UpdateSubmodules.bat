@echo off
echo Initializing git submodules...

git submodule update --init --recursive
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to initialize git submodules!
    pause
    exit /b 1
)
pause
exit
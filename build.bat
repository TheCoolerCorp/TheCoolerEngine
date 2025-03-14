@echo off
echo Initializing git submodules...

git submodule update --init --recursive
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to initialize git submodules!
    exit /b 1
)

echo Building Visual Studio solution...
setlocal

set PROJECT_NAME=TheCoolerEngine
set BUILD_DIR=out/build/VS_2022

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake --preset=VS_2022
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake configuration failed!
    popd
    exit /b 1
)

if not exist %PROJECT_NAME%.sln (
    echo [ERROR] Solution file not found: %PROJECT_NAME%.sln
    popd
    exit /b 1
)
start %PROJECT_NAME%.sln

popd
pause
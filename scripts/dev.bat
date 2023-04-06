@echo off

set /A startup=1

:dev
if %startup%==0 (
    set "userinp="
    set /p USERINP=[ENTER] to build and run again...
    if defined userinp (cls)
)
cls

echo -- Configuring CMake
cmake -DGLFW_BUILD_DOCS=OFF -S .. -B ../out/build -G "MinGW Makefiles"

echo -- Waiting for CMake to finish configuring
:loopConfigure
IF NOT EXIST ../out/build/Makefile (
    timeout /t 1 > nul
    GOTO loopConfigure
)

echo -- Building project
cd ../out/build
make

echo -- Waiting for executable to be built
:loopBuild
IF NOT EXIST Skinify.exe (
    timeout /t 1 > nul
    GOTO loopBuild
)

echo:
echo Running executable:
Skinify.exe

set /A startup=0
cd ..
GOTO dev

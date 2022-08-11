@echo off
SETLOCAL EnableDelayedExpansion

for %%x in (%*) do (
    set /A args+=1
)

if NOT %args%==2 (
  echo Passed wrong number of arguments.
  echo Correct usage: %setup.bat [size] [start position]
  echo Minimum size == 5.
)
if %1 LSS 5 (
    echo Minimum size must be equal 5.
    GOTO EXIT
)

set /A first=%1*%1-1
if %2 LSS 0 (
    echo Error: position smaller than 0.
    echo Position must be in range from 0 to %first%.
    GOTO EXIT
)
if %2 GEQ %first% (
    echo Error: position greater than %first%.
    echo Position must be in range from 0 to %first%.
    GOTO EXIT
)

for /f "delims=" %%A in ('cd') do (
    set foldername=%%~nxA
)

if NOT %foldername%==Knights-Tour (
    echo Your are in wrong directory. Make sure you run 'setup.bat' script in 'Knights-Tour' directory.
    GOTO EXIT
)

set DIR=%CD%
set DIR=%DIR:\=/%
echo %DIR%

if NOT exist %DIR%/.build (
    mkdir .build
    cd %DIR%/.build
    cmake ../ -G "MinGW Makefiles" -B . -S ../
    cd %DIR%
)
cmake --build %DIR%/.build
cd %DIR%/Algorithm/bin


echo Calculations in progress. It may take a while ...
KnightMain.exe %1 %2 | convert_output.exe > %DIR%/Algorithm/converted.txt

cd %DIR%/OpenGL/bin
main.exe %1 %2 < %DIR%/Algorithm/converted.txt

:EXIT
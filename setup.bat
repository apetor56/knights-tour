@echo off
SETLOCAL EnableDelayedExpansion

@REM checks number of arguments passed in command line
set /A args=0
for %%x in (%*) do (
    set /A args+=1
)

@REM if args number is not equal 2, then finish script
if NOT %args%==2 (
  echo Passed wrong number of arguments.
  echo Correct usage: ./setup.bat [size] [start position]
  echo Minimum size must be equal 5.
  GOTO EXIT
)

@REM checks whether size equals 5 at least
if %1 LSS 5 (
    echo Minimum size must be equal 5.
    GOTO EXIT
)

@REM checks if passed position is correct
set /A maxPos=%1*%1-1
if %2 LSS 0 (
    echo Error: position smaller than 0.
    echo Position must be in range from 0 to %maxPos%.
    GOTO EXIT
)
if %2 GEQ %maxPos% (
    echo Error: position greater than %maxPos%.
    echo Position must be in range from 0 to %maxPos%.
    GOTO EXIT
)

@REM get current folder name
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

@REM if build dir doesn't exist, then create it
if NOT exist %DIR%/build (
    mkdir build
    cd %DIR%/build
    cmake ../ -G "MinGW Makefiles" -B . -S ../
    cd %DIR%
)
@REM cmake build
cmake --build %DIR%/build
cd %DIR%/Algorithm/bin


echo Calculations in progress. It may take a while ...
@REM firstly run KnightMain.exe and then convert output to specific format
KnightMain.exe %1 %2 | convert_output.exe > %DIR%/Algorithm/converted.txt

@REM calculate output file size
set file=%DIR%/Algorithm/converted.txt
FOR /F "usebackq" %%A IN ('%file%') DO set size=%%~zA

@REM if size equals 0, then solution doesn't exist
if %size%==0 (
    echo Solution not found. Try with another position.
    GOTO EXIT
)

@REM if output file is correct, then OpenGL program is run
cd %DIR%/OpenGL/bin
main.exe %1 %2 < %DIR%/Algorithm/converted.txt

:EXIT
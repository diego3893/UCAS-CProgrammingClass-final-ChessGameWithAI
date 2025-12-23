@echo off

set "CMAKE_GENERATOR=MinGW Makefiles"
set "PROJECT_NAME=P2PChessGame"
set "TEST_NAME=test_rule"
set "MAKE_CMD=mingw32-make"

if exist "build" (
    rd /s /q "build"
)

mkdir build
cd build

cmake -G "%CMAKE_GENERATOR%" ..

%MAKE_CMD%

echo Run %PROJECT_NAME%.exe now?(Y/N)
set /p RUN_CHOICE=
if /i "%RUN_CHOICE%"=="Y" (
    start %PROJECT_NAME%.exe
)

echo Run %TEST_NAME%.exe now?(Y/N)
set /p RUN_CHOICE=
if /i "%RUN_CHOICE%"=="Y" (
    start %TEST_NAME%.exe
)

exit /b 0
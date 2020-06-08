@ECHO OFF
CLS
DEL main.exe
ECHO.
ECHO Building project...
ECHO.
g++                             ^
    ./src/*.cpp                 ^
    -o main.exe                 ^
    -I ./include                ^
    -L ./lib/SFML               ^
    -l sfml-graphics-s          ^
    -l sfml-window-s            ^
    -l sfml-system-s            ^
    -l opengl32                 ^
    -l winmm                    ^
    -l freetype                 ^
    -l gdi32
IF %ERRORLEVEL% NEQ 0 (
    ECHO Failed to build project. %ERRORLEVEL%
) ELSE (
    ECHO Successfully built project!
)
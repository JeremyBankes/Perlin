@ECHO OFF
CLS
ECHO.
main.exe
IF %ERRORLEVEL% NEQ 0 (
    ECHO Project exited with error code %ERRORLEVEL%
)
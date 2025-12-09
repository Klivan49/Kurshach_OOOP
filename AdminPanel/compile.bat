@echo off
chcp 65001 > nul
setlocal enabledelayedexpansion

echo.
echo ==========================================
echo Game Review Admin Panel - Compilation
echo Using: MySQL Connector/C 8.0 with GCC
echo ==========================================
echo.

echo [STEP 1/3] Cleaning old files...
del *.o 2>nul
del admin_panel.exe 2>nul
echo.

echo [STEP 2/3] Compilation...
echo.

REM IMPORTANT: Use MySQL Connector/C, NOT MySQL Server!
REM Download from: https://dev.mysql.com/downloads/connector/c/
REM Extract to: C:/Program Files/MySQL/MySQL Connector C 8.0/

set MYSQL_INCLUDE=C:/Program Files/MySQL/MySQL Connector C 9.5/include
set MYSQL_LIB=C:/Program Files/MySQL/MySQL Connector C 9.5/lib

g++ -std=c++17 -Wall -Wextra ^
  -I"%MYSQL_INCLUDE%" ^
  -L"%MYSQL_LIB%" ^
  -o admin_panel ^
  src/main.cpp ^
  src/Database.cpp ^
  src/User.cpp ^
  src/Game.cpp ^
  src/Menu.cpp ^
  src/AdminPanel.cpp ^
  -lmysqlclient -lws2_32 ^
  -static-libgcc -static-libstdc++

echo.

if %ERRORLEVEL% EQU 0 (
    echo [SUCCESS] Compilation complete!
    echo.
    echo [STEP 3/3] Running application...
    echo.
    admin_panel.exe
) else (
    echo [ERROR] Compilation failed!
    echo.
    echo TROUBLESHOOTING:
    echo 1. Ensure MySQL Connector/C 8.0 is installed correctly
    echo 2. Check that mysql.h exists at:
    echo    "%MYSQL_INCLUDE%\mysql.h"
    echo 3. Check that mysqlclient.lib exists at:
    echo    "%MYSQL_LIB%\mysqlclient.lib"
    echo.
    pause
    exit /b 1
)

pause

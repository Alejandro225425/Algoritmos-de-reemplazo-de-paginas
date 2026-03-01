@echo off
REM Script de compilación para Windows

echo Compilando Simulador de Algoritmos de Reemplazo de Páginas...
echo.

REM Compilar todos los archivos
g++ -std=c++17 -Wall -O2 -o simulador ^
    src/main.cpp ^
    src/FIFO.cpp ^
    src/LRU.cpp ^
    src/LFU.cpp ^
    src/MFU.cpp ^
    src/Optimal.cpp ^
    src/Clock.cpp ^
    src/SecondChance.cpp ^
    src/NRU.cpp ^
    -I./include

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✓ Compilación exitosa
    echo.
    echo Para ejecutar el programa, escribe: simulador.exe
) else (
    echo.
    echo ✗ Error en la compilación
    echo Asegúrate de tener g++ instalado y en el PATH
)

pause

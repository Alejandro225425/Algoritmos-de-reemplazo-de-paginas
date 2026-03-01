@echo off
REM Script para compilar y ejecutar el simulador

echo.
echo ========================================
echo  COMPILANDO SIMULADOR DE PAGINACION
echo ========================================
echo.

REM Cambiar al directorio del proyecto
cd /d "c:\Users\PREDATOR\Documents\Algoritmos de reemplazo"

REM Compilar el programa
echo Compilando...
g++ -std=c++17 -O2 -o simulador.exe ^
    src/main.cpp ^
    src/FIFO.cpp ^
    src/LRU.cpp ^
    src/LFU.cpp ^
    src/MFU.cpp ^
    src/Optimal.cpp ^
    src/Clock.cpp ^
    src/SecondChance.cpp ^
    src/NRU.cpp ^
    -I include

REM Verificar si la compilacion fue exitosa
if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo  COMPILACION EXITOSA
    echo ========================================
    echo.
    echo Ejecutando simulador...
    echo.
    simulador.exe
) else (
    echo.
    echo ========================================
    echo  ERROR EN COMPILACION
    echo ========================================
    pause
)

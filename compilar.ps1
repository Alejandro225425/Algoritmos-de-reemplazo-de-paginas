# Script de compilación para Windows PowerShell

Write-Host "Compilando Simulador de Algoritmos de Reemplazo de Páginas..." -ForegroundColor Cyan
Write-Host ""

# Verificar que g++ está disponible
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "✗ Error: g++ no se encuentra en el PATH" -ForegroundColor Red
    Write-Host "Por favor, instala MinGW o asegúrate de que está en el PATH" -ForegroundColor Yellow
    exit 1
}

# Compilar
$CompileCmd = @(
    "g++", "-std=c++17", "-Wall", "-O2", "-o", "simulador",
    "src/main.cpp",
    "src/FIFO.cpp",
    "src/LRU.cpp",
    "src/LFU.cpp",
    "src/MFU.cpp",
    "src/Optimal.cpp",
    "src/Clock.cpp",
    "src/SecondChance.cpp",
    "src/NRU.cpp",
    "-I./include"
)

& $CompileCmd[0] $CompileCmd[1..($CompileCmd.Length-1)]

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "✓ Compilación exitosa" -ForegroundColor Green
    Write-Host ""
    Write-Host "Para ejecutar el programa, escribe: .\simulador.exe" -ForegroundColor Yellow
} else {
    Write-Host ""
    Write-Host "✗ Error en la compilación" -ForegroundColor Red
}

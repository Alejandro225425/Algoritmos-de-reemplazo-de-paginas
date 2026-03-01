# Simulador de Algoritmos de Reemplazo de Páginas en Memoria

Este es un programa completo en C++ que implementa 8 algoritmos diferentes de reemplazo de páginas para sistemas de memoria virtual.

## Algoritmos Implementados

1. **FIFO (First In First Out)**: Reemplaza la página más antigua en memoria
2. **LRU (Least Recently Used)**: Reemplaza la página menos utilizada recientemente
3. **LFU (Least Frequently Used)**: Reemplaza la página con menor frecuencia de uso
4. **MFU (Most Frequently Used)**: Reemplaza la página con mayor frecuencia de uso
5. **Óptimo (Optimal)**: Reemplaza la página que se usará más lejano en el futuro
6. **Reloj (Clock)**: Implementa un algoritmo circular con bits de referencia
7. **Segunda Oportunidad (Second Chance)**: Variante mejorada de FIFO con bits de referencia
8. **NRU (Not Recently Used)**: Clasifica páginas según bits de referencia y modificación

## Características del Programa

- Interfaz de menú interactiva en consola
- Selección del algoritmo a utilizar
- Ingreso flexible de páginas y marcos de memoria
- Visualización de tablas de reemplazo (limitadas a 50 registros para no saturar pantalla)
- Métricas detalladas de rendimiento:
  - **Recursos Utilizados**: Unidades de recursos consumidas
  - **Tiempo de Ejecución**: Tiempo en microsegundos
  - **Llamadas al Sistema**: Total de syscalls generadas
  - **Interrupciones**: Interrupciones de hardware simuladas
  - **Tasa de Acierto/Fallo**: Porcentaje de hits y faults
  - **Promedios por Acceso**: Métricas normalizadas

## Compilación

### Linux/macOS

```bash
cd "c:\Users\PREDATOR\Documents\Algoritmos de reemplazo"
mkdir build
cd build
cmake ..
make
./simulador
```

### Windows (con CMake)

```bash
cd "c:\Users\PREDATOR\Documents\Algoritmos de reemplazo"
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build .
.\Debug\simulador.exe
```

### Windows (compilación manual con G++)

```bash
cd "c:\Users\PREDATOR\Documents\Algoritmos de reemplazo"
g++ -std=c++17 -o simulador src/main.cpp src/FIFO.cpp src/LRU.cpp src/LFU.cpp src/MFU.cpp src/Optimal.cpp src/Clock.cpp src/SecondChance.cpp src/NRU.cpp -I./include
simulador.exe
```

## Estructura del Proyecto

```
Algoritmos de reemplazo/
├── include/
│   ├── PagingAlgorithm.h      (Clase base abstracta)
│   ├── FIFO.h                  (Interfaz FIFO)
│   ├── LRU.h                   (Interfaz LRU)
│   ├── LFU.h                   (Interfaz LFU)
│   ├── MFU.h                   (Interfaz MFU)
│   ├── Optimal.h               (Interfaz Óptimo)
│   ├── Clock.h                 (Interfaz Reloj)
│   ├── SecondChance.h          (Interfaz Segunda Oportunidad)
│   └── NRU.h                   (Interfaz NRU)
├── src/
│   ├── main.cpp                (Programa principal)
│   ├── FIFO.cpp                (Implementación FIFO)
│   ├── LRU.cpp                 (Implementación LRU)
│   ├── LFU.cpp                 (Implementación LFU)
│   ├── MFU.cpp                 (Implementación MFU)
│   ├── Optimal.cpp             (Implementación Óptimo)
│   ├── Clock.cpp               (Implementación Reloj)
│   ├── SecondChance.cpp        (Implementación Segunda Oportunidad)
│   └── NRU.cpp                 (Implementación NRU)
├── CMakeLists.txt              (Configuración CMake)
└── README.md                   (Este archivo)
```

## Uso del Programa

1. Ejecuta el programa: `simulador` o `simulador.exe`
2. Selecciona un algoritmo del menú (1-8)
3. Ingresa la cantidad de marcos de memoria
4. Ingresa la secuencia de páginas a acceder (termina con -1)
5. El programa ejecuta la simulación y muestra:
   - Todas las métricas de rendimiento
   - Opción de ver el historial de reemplazos
   - Opción de ver la tabla de estados de los marcos

## Ejemplo de Uso

```
Seleccione un algoritmo:
1. FIFO (First In First Out)
...
Opción: 1

Ingrese la cantidad de marcos de memoria: 3

Ingrese la secuencia de páginas a acceder.
Ingrese -1 para terminar:
Página: 1
Página: 2
Página: 3
Página: 1
Página: 4
Página: 2
Página: 5
Página: -1
```

## Métricas Explicadas

### Accesos a Memoria
- **Page Hits**: Número de accesos donde la página ya estaba en memoria
- **Page Faults**: Número de accesos donde había que cargar la página
- **Tasa de Acierto**: Porcentaje de hits respecto al total

### Recursos Utilizados
Simulación del uso de recursos del sistema (CPU, bus de memoria, etc.)

### Tiempo de Ejecución
Medición precisa en microsegundos del tiempo total de simulación

### Llamadas al Sistema
- Cada fault genera ~2 syscalls (interrupt handling + memory fetch)
- Cada hit genera 1 syscall (simple read)

### Interrupciones
Número de interrupciones de hardware simuladas (1 por cada fault)

## Notas Técnicas

- El algoritmo Óptimo requiere conocer TODA la secuencia de accesos futuros
- Los bits de referencia se limpian cada 10 ciclos en NRU
- La visualización de tablas se limita a 50 registros para evitar saturar la pantalla
- Las métricas se calculan con precisión de punto flotante

## Recursos

- Para más información sobre algoritmos de reemplazo de páginas: 
  "Operating Systems Concepts" - Abraham Silberschatz et al.

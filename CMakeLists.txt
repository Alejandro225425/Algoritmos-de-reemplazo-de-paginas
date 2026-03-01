cmake_minimum_required(VERSION 3.10)
project(Algoritmos_Reemplazo_Paginas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Incluir directorios
include_directories(${PROJECT_SOURCE_DIR}/include)

# Archivos fuente
set(SOURCES
    src/main.cpp
    src/FIFO.cpp
    src/LRU.cpp
    src/LFU.cpp
    src/MFU.cpp
    src/Optimal.cpp
    src/Clock.cpp
    src/SecondChance.cpp
    src/NRU.cpp
)

# Crear ejecutable
add_executable(simulador ${SOURCES})

# Configuración de compilación para Windows
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <memory>
#include <iomanip>
#include <cstdio>
#include "FIFO.h"
#include "LRU.h"
#include "LFU.h"
#include "MFU.h"
#include "Optimal.h"
#include "Clock.h"
#include "SecondChance.h"
#include "NRU.h"

using namespace std;

class PagingSimulator {
private:
    unique_ptr<PagingAlgorithm> algorithm;
    vector<int> pageSequence;
    int numFrames = 0;
    
public:
    void clearScreen() {
        system("cls");
    }
    
    void showMainMenu() {
        clearScreen();
        cout << "========================================================\n";
        cout << "   SIMULADOR DE ALGORITMOS DE REEMPLAZO DE PAGINAS\n";
        cout << "========================================================\n\n";
        cout << "Seleccione un algoritmo:\n";
        cout << "1. FIFO (First In First Out)\n";
        cout << "2. LRU (Least Recently Used)\n";
        cout << "3. LFU (Least Frequently Used)\n";
        cout << "4. MFU (Most Frequently Used)\n";
        cout << "5. Optimo (Optimal)\n";
        cout << "6. Reloj (Clock)\n";
        cout << "7. Segunda Oportunidad (Second Chance)\n";
        cout << "8. NRU (Not Recently Used)\n";
        cout << "9. Salir\n";
        cout << "Opcion: ";
    }
    
    void selectAlgorithm(int choice) {
        switch(choice) {
            case 1:
                algorithm = make_unique<FIFO>();
                break;
            case 2:
                algorithm = make_unique<LRU>();
                break;
            case 3:
                algorithm = make_unique<LFU>();
                break;
            case 4:
                algorithm = make_unique<MFU>();
                break;
            case 5:
                algorithm = make_unique<Optimal>();
                break;
            case 6:
                algorithm = make_unique<Clock>();
                break;
            case 7:
                algorithm = make_unique<SecondChance>();
                break;
            case 8:
                algorithm = make_unique<NRU>();
                break;
            default:
                cout << "Opcion invalida\n";
                return;
        }
        if (algorithm) {
            runSimulation();
        }
    }
    
    void inputFrames() {
        cout << "\nIngrese la cantidad de marcos de memoria: ";
        cin >> numFrames;
        if (numFrames <= 0) {
            cout << "Debe ingresar un numero positivo.\n";
            inputFrames();
        }
    }
    
    void inputPages() {
        pageSequence.clear();
        cout << "\nIngrese la secuencia de paginas a acceder, separadas por espacios.\n";
        cout << "(Por ejemplo: 1 2 3 4 1 2 5 1 2 3 4 5)\n";
        cout << "Programa: ";
        
        // Limpiar buffer
        cin.ignore(10000, '\n');
        
        string line;
        getline(cin, line);
        
        stringstream ss(line);
        int page;
        while (ss >> page) {
            if (page >= 0) {
                pageSequence.push_back(page);
            }
        }
        
        if (pageSequence.empty()) {
            cout << "Debe ingresar al menos una pagina valida (no negativa).\n";
            inputPages();
        }
    }
    
    void runSimulation() {
        clearScreen();
        inputFrames();
        inputPages();
        
        if (dynamic_cast<Optimal*>(algorithm.get())) {
            dynamic_cast<Optimal*>(algorithm.get())->setFutureAccess(pageSequence);
        }
        
        algorithm->initialize(numFrames);
        
        auto startTime = chrono::high_resolution_clock::now();
        
        for (int page : pageSequence) {
            algorithm->accessPage(page);
        }
        
        auto endTime = chrono::high_resolution_clock::now();
        auto executionTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
        
        showResults(executionTime.count());
    }
    
    void showResults(long long executionTime) {
        clearScreen();
        const Metrics& metrics = algorithm->getMetrics();
        
        // Mostrar primero la tabla de estados inicial una vez
        showStateTable();
        
        while (true) {
            clearScreen();
            cout << "========================================================\n";
            cout << "                    MENU DE RESULTADOS                 \n";
            cout << "========================================================\n\n";
            
            cout << "Algoritmo: " << algorithm->getName() << "\n";
            cout << "Marcos de memoria: " << numFrames << "\n";
            cout << "Paginas accedidas: " << pageSequence.size() << "\n\n";
            
            cout << "1. Ver Tabla de Estados de Marco e Informacion Interna\n";
            cout << "2. Ver Metricas Detalladas e Informacion del Sistema\n";
            cout << "3. Volver al Menu Principal\n";
            cout << "\nOpcion: ";
            
            int opcion;
            if (!(cin >> opcion)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }
            // Limpiar el buffer despues de leer opcion
            cin.ignore(10000, '\n');
            
            if (opcion == 1) {
                showStateTable();
            } else if (opcion == 2) {
                clearScreen();
                showMetrics(metrics, executionTime);
                cout << "\nPresione Enter para volver al menu de resultados...";
                cin.get();
            } else if (opcion == 3) {
                break;
            }
        }
    }
    
    void showMetrics(const Metrics& metrics, long long executionTime) {
        cout << "\n========================================================\n";
        cout << "                    METRICAS DEL ALGORITMO\n";
        cout << "========================================================\n\n";
        
        // EFICIENCIA DEL ALGORITMO
        cout << "[1] EFICIENCIA DEL ALGORITMO (TASA DE ACIERTO/FALLO)\n";
        cout << "----------------------------------------------------\n";
        double hitRate = (100.0 * metrics.pageHits / metrics.totalAccesses);
        double faultRate = (100.0 * metrics.pageFaults / metrics.totalAccesses);
        
        cout << "  Accesos exitosos (Hits):      " << metrics.pageHits << " / " << metrics.totalAccesses << "\n";
        cout << "  Accesos fallidos (Faults):    " << metrics.pageFaults << " / " << metrics.totalAccesses << "\n";
        cout << "  [TASA DE ACIERTO]:            " << fixed << setprecision(1) << hitRate << "%";
        if (hitRate >= 75) cout << " [EXCELENTE]";
        else if (hitRate >= 50) cout << " [BUENO]";
        else if (hitRate >= 25) cout << " [REGULAR]";
        else cout << " [POBRE]";
        cout << "\n";
        cout << "  [TASA DE FALLO]:              " << faultRate << "%\n";
        cout << "  INTERPRETACION: Mientras mayor sea la tasa de acierto,\n";
        cout << "                  mejor el algoritmo predice accesos futuros\n\n";
        
        // CARGA DEL SISTEMA
        cout << "[2] CARGA DEL SISTEMA (RECURSOS UTILIZADOS)\n";
        cout << "----------------------------------------------------\n";
        cout << "  Recursos totales consumidos:  " << metrics.resourcesUsed << " unidades\n";
        cout << "  Promedio por acceso:          " << fixed << setprecision(2)
             << (double)metrics.resourcesUsed / metrics.totalAccesses << " unidades/acceso\n";
        cout << "\n  QUE SE MIDE INTERNAMENTE:\n";
        cout << "  - Ciclos de CPU para buscar pagina en tabla de marcos\n";
        cout << "  - Accesos a memoria para actualizar estructuras internas del algoritmo\n";
        cout << "  - Operaciones en cache L1/L2 (si hay fallo, se accesa memoria principal)\n";
        cout << "  - Iteraciones necesarias para encontrar marco victima\n";
        cout << "  - Accesos al TLB (Translation Lookaside Buffer) para traduccion virtual->fisica\n";
        cout << "  - Escrituras en disco (en fallos de pagina) si aplica\n";
        cout << "\n  INTERPRETACION: Menor consumo = algoritmo mas eficiente,\n";
        cout << "                  usa menos ciclos CPU y menos memoria\n\n";
        
        // RENDIMIENTO TEMPORAL
        cout << "[3] RENDIMIENTO TEMPORAL (VELOCIDAD DE EJECUCION)\n";
        cout << "----------------------------------------------------\n";
        cout << "  Tiempo total de simulacion:   " << executionTime << " microsegundos (" 
             << fixed << setprecision(3) << (executionTime / 1000.0) << " ms)\n";
        cout << "  Tiempo promedio por acceso:   " << fixed << setprecision(2)
             << (double)executionTime / metrics.totalAccesses << " microsegundos\n";
        cout << "\n  QUE SE MIDE INTERNAMENTE:\n";
        cout << "  - Latencia de acceso a cache (ciclos muy rapidos) en HIT\n";
        cout << "  - Latencia de acceso a RAM (decenas de ciclos) en FAULT con dato en memoria\n";
        cout << "  - Latencia de I/O a disco (~ms) en FAULT con pagina no cargada\n";
        cout << "  - Overhead de context switch (cambio de modo user->kernel->user)\n";
        cout << "  - Ciclos de CPU gastos en algoritmo de seleccion de victima\n";
        cout << "  - Tiempo de traducccion de direcciones virtuales a fisicas (TLB miss)\n";
        cout << "\n  INTERPRETACION: Menor tiempo = algoritmo con mejor prediccion,\n";
        cout << "                  evita accesos costosos a disco\n\n";
        
        // LLAMADAS AL SISTEMA
        cout << "[4] INTERACCION CON SISTEMA OPERATIVO (LLAMADAS AL SO)\n";
        cout << "----------------------------------------------------\n";
        cout << "  Total de llamadas al SO:      " << metrics.systemCalls << " llamadas\n";
        cout << "  Promedio por acceso:          " << fixed << setprecision(2)
             << (double)metrics.systemCalls / metrics.totalAccesses << " llamadas/acceso\n";
        cout << "\n  TIPOS DE LLAMADAS REALIZADAS EN ESTA SESION:\n";
        cout << "  - sys_page_fault (Manejo de excepcion por fallo): " << metrics.pageFaults << " llamadas\n";
        cout << "  - sys_io_read (Lectura desde disco a RAM):        " << metrics.pageFaults << " llamadas\n";
        cout << "  - sys_mprotect / acceso directo cache (Hits):     " << metrics.pageHits << " llamadas\n";
        cout << "\n  DESGLOSE MATEMATICO (Simulador):\n";
        cout << "  - FALLOS (Faults):  generan " << metrics.pageFaults << " fallos x 2 llamadas (fault + I/O read)\n";
        cout << "  - ACIERTOS (Hits):  generan " << metrics.pageHits << " aciertos x 1 llamada (validacion/acceso)\n";
        cout << "\n  INTERPRETACION: Menos syscalls = menos overhead, CPU gasta mas tiempo\n";
        cout << "                  en tareas utiles en lugar del kernel\n\n";
        
        // INTERRUPCIONES
        cout << "[5] INTERRUPCIONES DE HARDWARE\n";
        cout << "----------------------------------------------------\n";
        cout << "  Total de interrupciones:      " << metrics.interrupts << " interrupciones\n";
        cout << "  Promedio por acceso:          " << fixed << setprecision(3)
             << (double)metrics.interrupts / metrics.totalAccesses << " interrupciones/acceso\n";
        cout << "\n  TIPOS DE INTERRUPCIONES REALIZADAS EN ESTA SESION:\n";
        cout << "  - IRQ14/IRQ15 (Page Fault Exception): " << metrics.pageFaults << " interrupciones\n";
        cout << "\n  DESGLOSE MATEMATICO (Simulador):\n";
        cout << "  - 1 Fallo de Pagina (Fault) genero exactamente 1 interrupcion principal\n";
        cout << "  - Cada interrupcion obliga al CPU a cambiar de contexto (overhead)\n";
        cout << "\n  INTERPRETACION: Menos interrupciones = mejor cache locality,\n";
        cout << "                  menos cambios de contexto = mejor rendimiento global\n\n";
        
        // RESUMEN
        cout << "========================================================\n";
        cout << "                      RESUMEN EJECUTIVO\n";
        cout << "========================================================\n";
        cout << "  Algoritmo:       " << algorithm->getName() << "\n";
        cout << "  Configuracion:   " << numFrames << " marcos, " << pageSequence.size() << " accesos\n";
        cout << "  Desempeno:       " << fixed << setprecision(1) << hitRate << "% de eficiencia\n";
        cout << "  Costo:           " << metrics.systemCalls << " syscalls, " << metrics.interrupts << " interrupciones\n";
        cout << "  Velocidad:       " << fixed << setprecision(2) << (double)executionTime / metrics.totalAccesses 
             << " us/acceso\n";
        cout << "========================================================\n";
    }
    
    void showStateTable() {
        clearScreen();
        
        cout << "========================================================\n";
        cout << "               TABLA DE ESTADOS DE MARCO\n";
        cout << "========================================================\n\n";
        
        const auto& history = algorithm->getHistory();
        vector<int> currentFrames(numFrames, -1);
        
        cout << "  Paso | Acceso |";
        for (int i = 0; i < min(numFrames, 10); i++) {
            printf(" Marco%d |", i);
        }
        if (numFrames > 10) {
            cout << " ... |";
        }
        cout << " Faults | Hits\n";
        cout << "------+--------+";
        for (int i = 0; i < min(numFrames, 10); i++) {
            cout << "--------+";
        }
        if (numFrames > 10) {
            cout << "----+";
        }
        cout << "--------+------\n";
        
        int faults = 0, hits = 0;
        int displayLimit = min(30, (int)history.size());
        
        for (int i = 0; i < displayLimit; i++) {
            // Actualizar marcos
            if (history[i].frameNumber != -1) {
                currentFrames[history[i].frameNumber] = history[i].pageNumber;
            }
            
            if (history[i].reason == "Hit") hits++;
            else faults++;
            
            printf("%6d | %6d |", i + 1, history[i].pageNumber);
            
            for (int j = 0; j < min(numFrames, 10); j++) {
                if (currentFrames[j] == -1) {
                    cout << "  [ ]   |";
                } else {
                    printf("  [%2d]  |", currentFrames[j]);
                }
            }
            
            if (numFrames > 10) {
                cout << " ... |";
            }
            
            printf(" %6d | %5d\n", faults, hits);
        }
        
        if (history.size() > 30) {
            cout << "... (" << history.size() - 30 << " pasos mas)\n";
        }
        
        cout << "\n========================================================\n";
        cout << "            INFORMACION INTERNA (ESTADO FINAL)\n";
        cout << "========================================================\n\n";
        cout << algorithm->getAlgorithmSpecificTable() << "\n";
        
        cout << "\nPresione Enter para continuar...";
        getchar();
    }
    
    void run() {
        while (true) {
            showMainMenu();
            int choice;
            cin >> choice;
            
            if (choice == 9) {
                cout << "\n¡Gracias por usar el simulador!\n";
                break;
            }
            
            selectAlgorithm(choice);
        }
    }
};

int main() {
    // Asegurar que los buffers se sincronicen correctamente
    // ios_base::sync_with_stdio(false);
    // cin.tie(nullptr);
    
    PagingSimulator simulator;
    simulator.run();
    
    return 0;
}

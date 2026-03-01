#ifndef PAGING_ALGORITHM_H
#define PAGING_ALGORITHM_H

#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <queue>
#include <map>
#include <set>

// Estructura para almacenar metricas de ejecucion
struct Metrics {
    int pageHits = 0;
    int pageFaults = 0;
    int totalAccesses = 0;
    double hitRate = 0.0;
    double faultRate = 0.0;
    long long executionTime = 0;  // en microsegundos
    int systemCalls = 0;
    int interrupts = 0;
    int resourcesUsed = 0;
};

// Estructura para un reemplazo de pagina
struct PageReplacement {
    int pageNumber;
    int frameNumber;
    std::string reason;  // "Hit", "Fault", "Replaced"
};

// Clase base abstracta para algoritmos de reemplazo
class PagingAlgorithm {
public:
    virtual ~PagingAlgorithm() = default;
    
    // Inicializar el algoritmo
    virtual void initialize(int frames) = 0;
    
    // Procesar un acceso a pagina
    virtual void accessPage(int pageNumber) = 0;
    
    // Obtener metricas
    virtual const Metrics& getMetrics() const = 0;
    
    // Obtener historial de reemplazos
    virtual const std::vector<PageReplacement>& getHistory() const = 0;
    
    // Obtener el contenido actual de marcos
    virtual std::vector<int> getCurrentFrames() const = 0;
    
    // Obtener nombre del algoritmo
    virtual std::string getName() const = 0;
    
    // Resetear el algoritmo
    virtual void reset() = 0;
    
    // Mostrar estado actual
    virtual void showCurrentState() const = 0;
    
    // Obtener tabla especifica del algoritmo (informacion interna)
    virtual std::string getAlgorithmSpecificTable() const = 0;

protected:
    int numFrames = 0;
    std::vector<int> frames;
    std::set<int> allPages;
    Metrics metrics;
    std::vector<PageReplacement> history;
    
    void recordPageFault(int pageNumber, int frameNumber, const std::string& reason) {
        allPages.insert(pageNumber);
        metrics.pageFaults++;
        metrics.systemCalls += 2;  // Manejo de interrupt + busqueda en memoria secundaria
        metrics.interrupts++;
        metrics.totalAccesses++;
        metrics.resourcesUsed += (5 + frameNumber);
        
        PageReplacement pr;
        pr.pageNumber = pageNumber;
        pr.frameNumber = frameNumber;
        pr.reason = reason;
        history.push_back(pr);
    }
    
    void recordPageHit(int pageNumber) {
        allPages.insert(pageNumber);
        metrics.pageHits++;
        metrics.systemCalls++;
        metrics.totalAccesses++;
        metrics.resourcesUsed += 1;
        
        PageReplacement pr;
        pr.pageNumber = pageNumber;
        pr.frameNumber = -1;
        pr.reason = "Hit";
        history.push_back(pr);
    }
};

#endif // PAGING_ALGORITHM_H

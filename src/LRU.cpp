#include "LRU.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void LRU::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    lruQueue.clear();
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void LRU::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit
        recordPageHit(pageNumber);
        
        // Mover a la posicion mas reciente (final de la cola)
        auto queueIt = std::find(lruQueue.begin(), lruQueue.end(), pageNumber);
        if (queueIt != lruQueue.end()) {
            lruQueue.erase(queueIt);
        }
        lruQueue.push_back(pageNumber);
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            lruQueue.push_back(pageNumber);
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Reemplazar la pagina menos recientemente usada
            int lruPage = lruQueue.front();
            lruQueue.pop_front();
            
            frameToReplace = std::find(frames.begin(), frames.end(), lruPage) - frames.begin();
            frames[frameToReplace] = pageNumber;
            lruQueue.push_back(pageNumber);
            recordPageFault(pageNumber, frameToReplace, "LRU Replacement");
        }
    }
}

void LRU::reset() {
    initialize(numFrames);
}

void LRU::showCurrentState() const {
    std::cout << "\n=== Estado Actual LRU ===\n";
    std::cout << "Marcos: ";
    for (int f : frames) {
        if (f == -1) std::cout << "[ ] ";
        else std::cout << "[" << f << "] ";
    }
    std::cout << "\nOrden LRU (mas antiguo a mas reciente): ";
    for (int p : lruQueue) {
        std::cout << p << " ";
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string LRU::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (LRU) ===\n";
    ss << "Pagina | En Memoria? | Marco Actual\n";
    ss << "-------+-------------+-------------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            ss << "SI      |      " << (it - frames.begin()) << "\n";
        } else {
            ss << "NO      |      - \n";
        }
    }
    ss << "\nOrden de uso (Menos reciente a Mas reciente):\n[ ";
    for (int page : lruQueue) {
        ss << page << " ";
    }
    ss << "]\n";
    return ss.str();
}

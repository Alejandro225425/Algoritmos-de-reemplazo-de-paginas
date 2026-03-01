#include "FIFO.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void FIFO::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    while (!pageQueue.empty()) pageQueue.pop();
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void FIFO::accessPage(int pageNumber) {
    // Verificar si la pagina ya esta en memoria
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit - ya esta en memoria
        recordPageHit(pageNumber);
    } else {
        // Pagina fault - no esta en memoria
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            pageQueue.push(pageNumber);
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Todos los marcos estan llenos - reemplazar el mas antiguo (FIFO)
            int oldestPage = pageQueue.front();
            pageQueue.pop();
            
            frameToReplace = std::find(frames.begin(), frames.end(), oldestPage) - frames.begin();
            frames[frameToReplace] = pageNumber;
            pageQueue.push(pageNumber);
            recordPageFault(pageNumber, frameToReplace, "FIFO Replacement");
        }
    }
}

void FIFO::reset() {
    initialize(numFrames);
}

void FIFO::showCurrentState() const {
    std::cout << "\n=== Estado Actual FIFO ===\n";
    std::cout << "Marcos: ";
    for (int f : frames) {
        if (f == -1) std::cout << "[ ] ";
        else std::cout << "[" << f << "] ";
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string FIFO::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (FIFO) ===\n";
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
    ss << "\nPaginas en orden de llegada (frente a final):\n[ ";
    std::queue<int> temp = pageQueue;
    while(!temp.empty()) {
        ss << temp.front() << " ";
        temp.pop();
    }
    ss << "]\n";
    return ss.str();
}

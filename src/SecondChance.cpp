#include "SecondChance.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void SecondChance::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    pageQueue.clear();
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void SecondChance::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit - establecer bit de referencia
        recordPageHit(pageNumber);
        
        // Actualizar el bit de referencia en la cola
        for (auto& p : pageQueue) {
            if (p.first == pageNumber) {
                p.second = true;  // Establecer bit de referencia
                break;
            }
        }
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            pageQueue.push_back({pageNumber, false});
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Buscar una pagina con bit de referencia en 0 (Segunda oportunidad)
            while (true) {
                auto page = pageQueue.front();
                pageQueue.pop_front();
                
                if (!page.second) {
                    // No tiene referencia - reemplazar
                    frameToReplace = std::find(frames.begin(), frames.end(), page.first) - frames.begin();
                    frames[frameToReplace] = pageNumber;
                    pageQueue.push_back({pageNumber, false});
                    recordPageFault(pageNumber, frameToReplace, "Second Chance Replacement");
                    break;
                } else {
                    // Tiene referencia - dar segunda oportunidad
                    page.second = false;
                    pageQueue.push_back(page);
                }
            }
        }
    }
}

void SecondChance::reset() {
    initialize(numFrames);
}

void SecondChance::showCurrentState() const {
    std::cout << "\n=== Estado Actual Segunda Oportunidad ===\n";
    std::cout << "Marcos: ";
    for (int f : frames) {
        if (f == -1) std::cout << "[ ] ";
        else std::cout << "[" << f << "] ";
    }
    std::cout << "\nCola (pagina, referencia): ";
    for (const auto& p : pageQueue) {
        std::cout << "[" << p.first << "," << (p.second ? "1" : "0") << "] ";
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string SecondChance::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (Segunda Oportunidad) ===\n";
    ss << "Pagina | En Memoria? | Marco | Bit Ref (En Cola)\n";
    ss << "-------+-------------+-------+------------------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            int idx = it - frames.begin();
            bool refBit = false;
            for (const auto& p : pageQueue) {
                if (p.first == page) {
                    refBit = p.second;
                    break;
                }
            }
            ss << "SI      |   " << idx << "   |        " << refBit << "\n";
        } else {
            ss << "NO      |   -   |        - \n";
        }
    }
    ss << "\nOrden de llegada (Pagina : Bit Ref)\n";
    for (const auto& item : pageQueue) {
        ss << "[" << item.first << " : " << item.second << "] ";
    }
    ss << "\n";
    return ss.str();
}

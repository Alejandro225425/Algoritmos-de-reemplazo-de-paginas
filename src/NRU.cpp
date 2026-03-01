#include "NRU.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void NRU::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    referenceBits.assign(frameCount, false);
    modifiedBits.assign(frameCount, false);
    cycleCount = 0;
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void NRU::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit
        recordPageHit(pageNumber);
        int frameIndex = it - frames.begin();
        referenceBits[frameIndex] = true;
        modifiedBits[frameIndex] = true;
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            referenceBits[frameToReplace] = true;
            modifiedBits[frameToReplace] = false;
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Cada cierto tiempo, limpiar bits de referencia
            cycleCount++;
            if (cycleCount % 10 == 0) {
                std::fill(referenceBits.begin(), referenceBits.end(), false);
            }
            
            // Encontrar pagina con menor clase NRU
            // Clase 0: (0,0), Clase 1: (0,1), Clase 2: (1,0), Clase 3: (1,1)
            int bestClass = 4;
            int nruPage = -1;
            
            for (int i = 0; i < numFrames; i++) {
                int pageClass = (referenceBits[i] ? 2 : 0) + (modifiedBits[i] ? 1 : 0);
                if (pageClass < bestClass) {
                    bestClass = pageClass;
                    nruPage = frames[i];
                }
            }
            
            frameToReplace = std::find(frames.begin(), frames.end(), nruPage) - frames.begin();
            frames[frameToReplace] = pageNumber;
            referenceBits[frameToReplace] = true;
            modifiedBits[frameToReplace] = false;
            recordPageFault(pageNumber, frameToReplace, "NRU Replacement");
        }
    }
}

void NRU::reset() {
    initialize(numFrames);
}

void NRU::showCurrentState() const {
    std::cout << "\n=== Estado Actual NRU ===\n";
    std::cout << "Marcos: ";
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == -1) std::cout << "[ ] ";
        else std::cout << "[" << frames[i] << "] ";
    }
    std::cout << "\n(R,M): ";
    for (int i = 0; i < numFrames; i++) {
        std::cout << "(" << (referenceBits[i] ? "1" : "0") << ","
                  << (modifiedBits[i] ? "1" : "0") << ") ";
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string NRU::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (NRU) ===\n";
    ss << "Pagina | En Memoria? | Marco | Clase | Ref (R) | Mod (M)\n";
    ss << "-------+-------------+-------+-------+---------+---------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            int i = it - frames.begin();
            int clase = (referenceBits[i] << 1) | modifiedBits[i];
            ss << "SI      |   " << i << "   |   " << clase << "   |    " 
               << referenceBits[i] << "    |    " << modifiedBits[i] << "    \n";
        } else {
            ss << "NO      |   -   |   -   |    -    |    -    \n";
        }
    }
    return ss.str();
}

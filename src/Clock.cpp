#include "Clock.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void Clock::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    referenceBits.assign(frameCount, false);
    clockPointer = 0;
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void Clock::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit - establecer bit de referencia
        recordPageHit(pageNumber);
        int frameIndex = it - frames.begin();
        referenceBits[frameIndex] = true;
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            referenceBits[frameToReplace] = true;
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Buscar un marco con bit de referencia en 0
            while (referenceBits[clockPointer]) {
                referenceBits[clockPointer] = false;
                clockPointer = (clockPointer + 1) % numFrames;
            }
            
            frameToReplace = clockPointer;
            frames[frameToReplace] = pageNumber;
            referenceBits[frameToReplace] = true;
            recordPageFault(pageNumber, frameToReplace, "Clock Replacement");
            
            clockPointer = (clockPointer + 1) % numFrames;
        }
    }
}

void Clock::reset() {
    initialize(numFrames);
}

void Clock::showCurrentState() const {
    std::cout << "\n=== Estado Actual Reloj ===\n";
    std::cout << "Marcos: ";
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == -1) std::cout << "[ ] ";
        else std::cout << "[" << frames[i] << "] ";
    }
    std::cout << "\nBits de referencia: ";
    for (int i = 0; i < numFrames; i++) {
        std::cout << (referenceBits[i] ? "1" : "0") << " ";
    }
    std::cout << "\nPuntero reloj: " << clockPointer;
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string Clock::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (Reloj) ===\n";
    ss << "Pagina | En Memoria? | Marco | Bit Ref | Puntero\n";
    ss << "-------+-------------+-------+---------+--------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            int idx = it - frames.begin();
            ss << "SI      |   " << idx << "   |    " << referenceBits[idx] << "    |   ";
            if (idx == clockPointer) ss << "<--";
            ss << "\n";
        } else {
            ss << "NO      |   -   |    -    |   \n";
        }
    }
    return ss.str();
}

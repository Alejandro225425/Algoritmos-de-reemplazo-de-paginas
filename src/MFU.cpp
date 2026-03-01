#include "MFU.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void MFU::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    frequency.clear();
    metrics = Metrics();
    history.clear();
    allPages.clear();
}

void MFU::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit
        recordPageHit(pageNumber);
        frequency[pageNumber]++;
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            frequency[pageNumber] = 1;
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Encontrar la pagina con mayor frecuencia
            int maxFreq = -1;
            int mfuPage = -1;
            
            for (int i = 0; i < numFrames; i++) {
                if (frames[i] != -1) {
                    int freq = frequency[frames[i]];
                    if (freq > maxFreq) {
                        maxFreq = freq;
                        mfuPage = frames[i];
                    }
                }
            }
            
            frameToReplace = std::find(frames.begin(), frames.end(), mfuPage) - frames.begin();
            frequency.erase(mfuPage);
            frames[frameToReplace] = pageNumber;
            frequency[pageNumber] = 1;
            recordPageFault(pageNumber, frameToReplace, "MFU Replacement");
        }
    }
}

void MFU::reset() {
    initialize(numFrames);
}

void MFU::showCurrentState() const {
    std::cout << "\n=== Estado Actual MFU ===\n";
    std::cout << "Marcos: ";
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] == -1) std::cout << "[ ] ";
        else std::cout << "[" << frames[i] << "] ";
    }
    std::cout << "\nFrecuencias: ";
    for (int i = 0; i < numFrames; i++) {
        if (frames[i] != -1) {
            std::cout << frames[i] << ":" << frequency.at(frames[i]) << " ";
        }
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string MFU::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (MFU) ===\n";
    ss << "Pagina | En Memoria? | Marco | Frecuencia Actual\n";
    ss << "-------+-------------+-------+------------------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            ss << "SI      |   " << (it - frames.begin()) << "   |        " << frequency.at(page) << "\n";
        } else {
            ss << "NO      |   -   |        0 (Desalojada)\n";
        }
    }
    return ss.str();
}

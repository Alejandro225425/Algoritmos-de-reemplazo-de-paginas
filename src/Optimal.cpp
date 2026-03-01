#include "Optimal.h"
#include <sstream>
#include <algorithm>
#include <iostream>

void Optimal::initialize(int frameCount) {
    numFrames = frameCount;
    frames.assign(frameCount, -1);
    metrics = Metrics();
    history.clear();
    futureAccess.clear();
    accessIndex = 0;
    allPages.clear();
}

void Optimal::accessPage(int pageNumber) {
    auto it = std::find(frames.begin(), frames.end(), pageNumber);
    
    if (it != frames.end()) {
        // Pagina hit
        recordPageHit(pageNumber);
        accessIndex++;
    } else {
        // Pagina fault
        int frameToReplace = -1;
        
        if (std::find(frames.begin(), frames.end(), -1) != frames.end()) {
            // Hay un marco vacio
            frameToReplace = std::find(frames.begin(), frames.end(), -1) - frames.begin();
            frames[frameToReplace] = pageNumber;
            recordPageFault(pageNumber, frameToReplace, "Empty Frame");
        } else {
            // Encontrar la pagina que se usara mas lejano en el futuro
            int maxDist = -1;
            int optimalPage = frames[0];
            
            for (int i = 0; i < numFrames; i++) {
                int distance = INT_MAX;
                
                // Buscar la proxima ocurrencia en accesos futuros
                for (int j = accessIndex + 1; j < futureAccess.size(); j++) {
                    if (futureAccess[j] == frames[i]) {
                        distance = j - accessIndex;
                        break;
                    }
                }
                
                if (distance > maxDist) {
                    maxDist = distance;
                    optimalPage = frames[i];
                }
            }
            
            frameToReplace = std::find(frames.begin(), frames.end(), optimalPage) - frames.begin();
            frames[frameToReplace] = pageNumber;
            recordPageFault(pageNumber, frameToReplace, "Optimal Replacement");
        }
        
        accessIndex++;
    }
}

void Optimal::reset() {
    initialize(numFrames);
}

void Optimal::showCurrentState() const {
    std::cout << "\n=== Estado Actual optimo ===\n";
    std::cout << "Marcos: ";
    for (int f : frames) {
        if (f == -1) std::cout << "[ ] ";
        else std::cout << "[" << f << "] ";
    }
    std::cout << "\nFaults: " << metrics.pageFaults << " | Hits: " << metrics.pageHits << "\n";
}

std::string Optimal::getAlgorithmSpecificTable() const {
    std::stringstream ss;
    ss << "=== Tabla Especifica: Estado de Todas las Paginas (Optimo) ===\n";
    ss << "Pagina | En Memoria? | Marco | Distancia a proximo uso\n";
    ss << "-------+-------------+-------+------------------------\n";
    for (int page : allPages) {
        auto it = std::find(frames.begin(), frames.end(), page);
        ss << "  " << page << "    |     ";
        if (it != frames.end()) {
            int distance = -1;
            for (int j = accessIndex; j < futureAccess.size(); j++) {
                if (futureAccess[j] == page) {
                    distance = j - accessIndex;
                    break;
                }
            }
            ss << "SI      |   " << (it - frames.begin()) << "   |        ";
            if (distance != -1) ss << distance << "\n";
            else ss << "No se usara mas\n";
        } else {
            ss << "NO      |   -   |        - \n";
        }
    }
    ss << "\nSiguientes accesos programados:\n[ ";
    for (size_t i = accessIndex; i < futureAccess.size(); ++i) {
        ss << futureAccess[i] << " ";
    }
    ss << "]\n";
    return ss.str();
}

#ifndef NRU_H
#define NRU_H

#include "PagingAlgorithm.h"

class NRU : public PagingAlgorithm {
private:
    std::vector<bool> referenceBits;  // Bit de referencia
    std::vector<bool> modifiedBits;   // Bit de modificacion
    int cycleCount = 0;
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "NRU (Not Recently Used)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // NRU_H

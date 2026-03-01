#ifndef CLOCK_H
#define CLOCK_H

#include "PagingAlgorithm.h"

class Clock : public PagingAlgorithm {
private:
    std::vector<bool> referenceBits;  // Bit de referencia para cada marco
    int clockPointer = 0;  // Puntero del reloj
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "Reloj (Clock)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // CLOCK_H

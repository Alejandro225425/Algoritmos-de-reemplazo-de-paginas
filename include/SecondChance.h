#ifndef SECONDCHANCE_H
#define SECONDCHANCE_H

#include "PagingAlgorithm.h"
#include <deque>

class SecondChance : public PagingAlgorithm {
private:
    std::deque<std::pair<int, bool>> pageQueue;  // pagina, bit de referencia
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "Segunda Oportunidad (Second Chance)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // SECONDCHANCE_H

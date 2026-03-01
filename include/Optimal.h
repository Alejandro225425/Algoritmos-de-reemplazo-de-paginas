#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "PagingAlgorithm.h"
#include <vector>

class Optimal : public PagingAlgorithm {
private:
    std::vector<int> futureAccess;  // Accesos futuros a las paginas
    int accessIndex = 0;
    
public:
    void initialize(int frames) override;
    void setFutureAccess(const std::vector<int>& access) { futureAccess = access; accessIndex = 0; }
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "optimo (Optimal)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // OPTIMAL_H

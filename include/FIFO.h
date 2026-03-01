#ifndef FIFO_H
#define FIFO_H

#include "PagingAlgorithm.h"
#include <queue>

class FIFO : public PagingAlgorithm {
private:
    std::queue<int> pageQueue;
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "FIFO (First In First Out)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // FIFO_H

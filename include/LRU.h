#ifndef LRU_H
#define LRU_H

#include "PagingAlgorithm.h"
#include <deque>

class LRU : public PagingAlgorithm {
private:
    std::deque<int> lruQueue;
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "LRU (Least Recently Used)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // LRU_H

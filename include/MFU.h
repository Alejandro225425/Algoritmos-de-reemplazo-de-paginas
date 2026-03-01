#ifndef MFU_H
#define MFU_H

#include "PagingAlgorithm.h"
#include <map>

class MFU : public PagingAlgorithm {
private:
    std::map<int, int> frequency;  // pagina -> frecuencia
    
public:
    void initialize(int frames) override;
    void accessPage(int pageNumber) override;
    const Metrics& getMetrics() const override { return metrics; }
    const std::vector<PageReplacement>& getHistory() const override { return history; }
    std::vector<int> getCurrentFrames() const override { return frames; }
    std::string getName() const override { return "MFU (Most Frequently Used)"; }
    void reset() override;
    void showCurrentState() const override;
    std::string getAlgorithmSpecificTable() const override;
};

#endif // MFU_H

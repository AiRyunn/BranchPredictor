#ifndef GSHARE_HPP
#define GSHARE_HPP

#include "predictor.hpp"

// Gshare predictor
// Memory usage: 2^gHistoryBits bytes
class GsharePredictor : public BasePredictor {
private:
    uint32_t GHR;             // Global history register
    std::vector<uint8_t> BHT; // Branch history table (2-bit saturating counter)
    const uint32_t indexMask; // Mask of the index
public:
    GsharePredictor(uint32_t gHistoryBits);
    uint8_t make_prediction(uint32_t pc) const override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

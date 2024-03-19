#ifndef BIMODEL_HPP
#define BIMODEL_HPP

#include "predictor.hpp"

class BimodelPredictor : public BasePredictor {
private:
    std::vector<uint8_t> BHT; // Branch history table (2-bit saturating counter)
    uint32_t indexMask;       // Mask to extract relevant bits
public:
    BimodelPredictor(uint32_t lHistoryBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

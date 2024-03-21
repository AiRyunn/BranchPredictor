#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include "predictor.hpp"

// Perceptron predictor: https://www.cs.utexas.edu/~lin/papers/hpca01.pdf
// Memory usage: 2*2^pcIndexBits * (gHistoryBits + 1) bytes
class PerceptronPredictor : public BasePredictor {
private:
    std::vector<std::vector<int16_t> > weights; // Perceptron weights
    uint64_t GHR;                               // Global History Register, as a 64-bit integer
    const uint32_t threshold;                   // Threshold for updating weights
    const uint32_t indexMask;                   // Mask of the index

    int calculate_sum(uint32_t index) const;

public:
    PerceptronPredictor(uint32_t pcIndexBits, uint32_t gHistoryBits, uint32_t threshold);
    uint8_t make_prediction(uint32_t pc) const override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

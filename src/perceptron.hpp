#ifndef PERCEPTRON_HPP
#define PERCEPTRON_HPP

#include "predictor.hpp"

// Perceptron predictor: https://www.cs.utexas.edu/~lin/papers/hpca01.pdf
class PerceptronPredictor : public BasePredictor {
private:
    std::vector<std::vector<int> > weights; // Perceptron weights
    std::vector<int> GHR;                   // Global History Register, as a vector of -1 (not taken) and 1 (taken)
    uint32_t threshold;
    uint32_t gHistoryBits;
    uint32_t indexMask;

public:
    PerceptronPredictor(uint32_t pcIndexBits, uint32_t gHistoryBits, uint32_t threshold);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

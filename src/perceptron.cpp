#include "perceptron.hpp"
#include <algorithm>
#include <climits>

PerceptronPredictor::PerceptronPredictor(uint32_t pcIndexBits, uint32_t gHistoryBits, uint32_t threshold)
    : GHR(0), threshold(threshold), indexMask((1 << pcIndexBits) - 1) {
    weights.resize(1 << pcIndexBits, std::vector<int16_t>(gHistoryBits + 1, 0)); // +1 for bias weight
}

int PerceptronPredictor::calculate_sum(uint32_t index) const {
    int sum = weights[index][0];
    for (size_t i = 1; i < weights[index].size(); ++i) {
        sum += weights[index][i] * ((GHR >> (i - 1)) & 1 ? 1 : -1);
    }
    return sum;
}

uint8_t PerceptronPredictor::make_prediction(uint32_t pc) const {
    uint32_t index = pc & indexMask;
    return calculate_sum(index) >= 0 ? TAKEN : NOTTAKEN;
}

void PerceptronPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    int target = outcome == TAKEN ? 1 : -1;
    int prediction = make_prediction(pc) == TAKEN ? 1 : -1;
    // Update weights if prediction was wrong or confidence is low
    if (prediction != target || std::abs(calculate_sum(index)) <= threshold) {
        // Update bias weight
        weights[index][0] += target;
        // Update based on GHR
        for (size_t i = 1; i < weights[index].size(); ++i) {
            weights[index][i] += target * ((GHR >> (i - 1)) & 1 ? 1 : -1);
        }
    }
    // Update GHR
    GHR = (GHR << 1) | outcome;
}

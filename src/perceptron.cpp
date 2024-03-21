#include "perceptron.hpp"
#include <algorithm>

PerceptronPredictor::PerceptronPredictor(uint32_t pcIndexBits, uint32_t gHistoryBits, uint32_t threshold)
    : threshold(threshold) {
    weights.resize(1 << pcIndexBits, std::vector<int>(gHistoryBits + 1, 0)); // +1 for bias weight
    GHR.resize(gHistoryBits, WT);                                            // Initialize with weakly taken
    indexMask = (1 << pcIndexBits) - 1;
}

uint8_t PerceptronPredictor::make_prediction(uint32_t pc) {
    uint32_t index = pc & indexMask;
    int sum = weights[index][0] + std::inner_product(weights[index].begin() + 1, weights[index].end(), GHR.begin(), 0);
    return sum >= 0 ? TAKEN : NOTTAKEN;
}

void PerceptronPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    int target = outcome == TAKEN ? 1 : -1;
    int prediction = make_prediction(pc) == TAKEN ? 1 : -1;
    if (prediction != target ||
        std::abs(weights[index][0] +
                 std::inner_product(weights[index].begin() + 1, weights[index].end(), GHR.begin(), 0)) <= threshold) {
        // Update weights if prediction was wrong or confidence is low
        for (size_t i = 0; i < weights[index].size(); ++i) {
            if (i == 0) {
                // Update bias weight
                weights[index][i] += target;
            } else {
                // Update based on GHR
                weights[index][i] += target * GHR[i - 1];
            }
        }
    }
    // Update GHR
    GHR.pop_back();                  // Remove the oldest history
    GHR.insert(GHR.begin(), target); // Add the newest outcome at the beginning
}

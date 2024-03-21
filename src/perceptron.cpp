#include "perceptron.hpp"

PerceptronPredictor::PerceptronPredictor(uint32_t pcIndexBits, uint32_t gHistoryBits, uint32_t threshold)
    : GHR(0), threshold(threshold), indexMask((1 << pcIndexBits) - 1) {
    weights.resize(1 << pcIndexBits, std::vector<int8_t>(gHistoryBits + 1, 0)); // +1 for bias weight
}

int PerceptronPredictor::calculate_sum(uint32_t index) const {
    int sum = weights[index][0];
    for (size_t i = 1; i < weights[index].size(); ++i) {
        if (GHR.test(i - 1)) {
            sum += weights[index][i];
        } else {
            sum -= weights[index][i];
        }
    }
    return sum;
}

uint8_t PerceptronPredictor::make_prediction(uint32_t pc) const {
    uint32_t index = pc & indexMask;
    return calculate_sum(index) >= 0 ? TAKEN : NOTTAKEN;
}

void PerceptronPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    int sum = calculate_sum(index);
    // Update weights if prediction was wrong or close to threshold
    if ((sum >= 0) != outcome || std::abs(sum) <= threshold) {
        for (size_t i = 0; i < weights[index].size(); ++i) {
            if (i == 0 && outcome == TAKEN || i > 0 && GHR.test(i - 1) == outcome) {
                if (weights[index][i] < INT8_MAX) {
                    weights[index][i]++;
                }
            } else {
                if (weights[index][i] > INT8_MIN) {
                    weights[index][i]--;
                }
            }
        }
    }
    // Update GHR
    GHR <<= 1;
    GHR.set(0, outcome);
}

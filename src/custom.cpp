#include "custom.hpp"

CustomPredictor::CustomPredictor() {
}

uint8_t CustomPredictor::make_prediction(uint32_t pc) {
    return NOTTAKEN;
}

void CustomPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
}

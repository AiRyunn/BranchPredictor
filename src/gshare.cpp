#include "gshare.hpp"

GsharePredictor::GsharePredictor(uint32_t gHistoryBits) : GHR(0), indexMask((1 << gHistoryBits) - 1) {
    BHT.resize(1 << gHistoryBits, WN); // Initialize with weakly not taken
}

uint8_t GsharePredictor::make_prediction(uint32_t pc) const {
    uint32_t index = (pc ^ GHR) & indexMask;
    return BHT[index] >= WT ? TAKEN : NOTTAKEN;
}

void GsharePredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = (pc ^ GHR) & indexMask;
    uint8_t &state = BHT[index];
    if (outcome == TAKEN && state < ST) {
        state++;
    } else if (outcome == NOTTAKEN && state > SN) {
        state--;
    }
    GHR = ((GHR << 1) | outcome) & indexMask;
}

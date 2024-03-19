#include "gshare.hpp"

GsharePredictor::GsharePredictor(uint32_t gHistoryBits)
    : GHR(0) {
    BHT.resize(1 << gHistoryBits, WT); // Initialize with weakly taken
    indexMask = (1 << gHistoryBits) - 1;
}

uint8_t GsharePredictor::make_prediction(uint32_t pc) {
    uint32_t index = (pc ^ GHR) & indexMask;
    return BHT[index] >= WT ? TAKEN : NOTTAKEN;
}

void GsharePredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = (pc ^ GHR) & indexMask;
    if (outcome == TAKEN && BHT[index] < ST) {
        BHT[index]++;
    } else if (outcome == NOTTAKEN && BHT[index] > SN) {
        BHT[index]--;
    }
    GHR = (GHR << 1) | outcome;
}

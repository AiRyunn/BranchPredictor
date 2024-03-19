#include "bimodel.hpp"

BimodelPredictor::BimodelPredictor(uint32_t lHistoryBits) {
    BHT.resize(1 << lHistoryBits, WT); // Initialize with weakly taken
    indexMask = (1 << lHistoryBits) - 1;
}

uint8_t BimodelPredictor::make_prediction(uint32_t pc) {
    uint32_t index = pc & indexMask;
    return BHT[index] >= WT ? TAKEN : NOTTAKEN;
}

void BimodelPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    if (outcome == TAKEN && BHT[index] < ST) {
        BHT[index]++;
    } else if (outcome == NOTTAKEN && BHT[index] > SN) {
        BHT[index]--;
    }
}

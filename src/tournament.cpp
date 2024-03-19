#include "tournament.hpp"
#include "gshare.hpp"
#include "bimodel.hpp"

TournamentPredictor::TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits) {
    gPredictor = new GsharePredictor(gHistoryBits);
    lPredictor = new BimodelPredictor(lHistoryBits);
    chooser.resize(1 << pcIndexBits, WN); // Initialize with weakly not taken
    indexMask = (1 << pcIndexBits) - 1;
}

uint8_t TournamentPredictor::make_prediction(uint32_t pc) {
    uint32_t index = pc & indexMask;
    if (chooser[index] >= WT) {
        return gPredictor->make_prediction(pc);
    } else {
        return lPredictor->make_prediction(pc);
    }
}

void TournamentPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    uint8_t gPrediction = gPredictor->make_prediction(pc);
    uint8_t lPrediction = lPredictor->make_prediction(pc);
    if (gPrediction != lPrediction) {
        if (gPrediction == outcome && chooser[index] < ST) {
            chooser[index]++;
        } else if (lPrediction == outcome && chooser[index] > SN) {
            chooser[index]--;
        }
    }
    gPredictor->train_predictor(pc, outcome);
    lPredictor->train_predictor(pc, outcome);
}

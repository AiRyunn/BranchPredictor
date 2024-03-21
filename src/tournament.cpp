#include "tournament.hpp"

LocalPredictor::LocalPredictor(uint32_t lHistoryBits, uint32_t pcIndexBits) {
    LPT.resize(1 << pcIndexBits, 0); // Initialize with 0
    BHT.resize(1 << lHistoryBits, WN); // Initialize with weakly not taken
    indexMask = (1 << pcIndexBits) - 1;
    lHistoryMask = (1 << lHistoryBits) - 1;
}

uint8_t LocalPredictor::make_prediction(uint32_t pc) {
    uint32_t index = pc & indexMask;
    return BHT[LPT[index]] >= WT ? TAKEN : NOTTAKEN;
}

void LocalPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    uint32_t &localPrediction = LPT[index];
    uint8_t &state = BHT[localPrediction & lHistoryMask];
    if (outcome == TAKEN && state < ST) {
        state++;
    } else if (outcome == NOTTAKEN && state > SN) {
        state--;
    }
    localPrediction = (localPrediction << 1 | outcome) & lHistoryMask;
}

GlobalPredictor::GlobalPredictor(uint32_t gHistoryBits) : GHR(0) {
    GHT.resize(1 << gHistoryBits, WN); // Initialize with weakly not taken
    gHistoryMask = (1 << gHistoryBits) - 1;
}

uint8_t GlobalPredictor::make_prediction(uint32_t pc) {
    return GHT[GHR] >= WT ? TAKEN : NOTTAKEN;
}

void GlobalPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    if (outcome == TAKEN && GHT[GHR] < ST) {
        GHT[GHR]++;
    } else if (outcome == NOTTAKEN && GHT[GHR] > SN) {
        GHT[GHR]--;
    }
    GHR = (GHR << 1 | outcome) & gHistoryMask;
}

TournamentPredictor::TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits) : GHR(0) {
    lPredictor = new LocalPredictor(lHistoryBits, pcIndexBits);
    gPredictor = new GlobalPredictor(gHistoryBits);
    chooser.resize(1 << pcIndexBits, WT); // Initialize with weakly taken to prefer global predictor
    gHistoryMask = (1 << gHistoryBits) - 1;
}

uint8_t TournamentPredictor::make_prediction(uint32_t pc) {
    if (chooser[GHR] >= WT) {
        return gPredictor->make_prediction(pc);
    } else {
        return lPredictor->make_prediction(pc);
    }
}

void TournamentPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint8_t gPrediction = gPredictor->make_prediction(pc);
    uint8_t lPrediction = lPredictor->make_prediction(pc);
    uint8_t &state = chooser[GHR];
    if (gPrediction != lPrediction) {
        if (gPrediction == outcome && state < ST) {
            state++;
        } else if (lPrediction == outcome && state > SN) {
            state--;
        }
    }
    gPredictor->train_predictor(pc, outcome);
    lPredictor->train_predictor(pc, outcome);
}

//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include "predictor.hpp"
#include <cassert>
#include <stdio.h>

//
// TODO:Student Information
//
const char *studentName = "NAME";
const char *studentID = "PID";
const char *email = "EMAIL";

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//

// Handy Global for use in output routines
const char *bpName[4] = {"Static", "Gshare", "Tournament", "Custom"};

int ghistoryBits; // Number of bits used for Global History
int lhistoryBits; // Number of bits used for Local History
int pcIndexBits;  // Number of bits used for PC index
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
// TODO: Add your own Branch Predictor data structures here
//

GsharePredictor::GsharePredictor(uint32_t gHistoryBits)
    : GHR(0), gHistoryBits(gHistoryBits) {
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

TournamentPredictor::TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits) {
    gshare = new GsharePredictor(gHistoryBits);
    bimodal = new BimodelPredictor(lHistoryBits);
    chooser.resize(1 << pcIndexBits, WN); // Initialize with weakly not taken
    indexMask = (1 << pcIndexBits) - 1;
}

uint8_t TournamentPredictor::make_prediction(uint32_t pc) {
    uint32_t index = pc & indexMask;
    if (chooser[index] >= WT) {
        return gshare->make_prediction(pc);
    } else {
        return bimodal->make_prediction(pc);
    }
}

void TournamentPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
    uint32_t index = pc & indexMask;
    uint8_t gsharePrediction = gshare->make_prediction(pc);
    uint8_t bimodalPrediction = bimodal->make_prediction(pc);
    if (gsharePrediction != bimodalPrediction) {
        if (gsharePrediction == outcome && chooser[index] < ST) {
            chooser[index]++;
        } else if (bimodalPrediction == outcome && chooser[index] > SN) {
            chooser[index]--;
        }
    }
    gshare->train_predictor(pc, outcome);
    bimodal->train_predictor(pc, outcome);
}

CustomPredictor::CustomPredictor() {
}

uint8_t CustomPredictor::make_prediction(uint32_t pc) {
    return NOTTAKEN;
}

void CustomPredictor::train_predictor(uint32_t pc, uint8_t outcome) {
}

BasePredictor *predictor;

//------------------------------------//
//        Predictor Functions         //
//------------------------------------//

// Initialize the predictor
//
void init_predictor() {
    //
    // TODO: Initialize Branch Predictor Data Structures
    //
    switch (bpType) {
    case STATIC:
        break;
    case GSHARE:
        predictor = new GsharePredictor(ghistoryBits);
        break;
    case TOURNAMENT:
        predictor = new TournamentPredictor(ghistoryBits, lhistoryBits, pcIndexBits);
        break;
    case CUSTOM:
        predictor = new CustomPredictor();
        break;
    default:
        break;
    }
}

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc) {
    //
    // TODO: Implement prediction scheme
    //

    // Make a prediction based on the bpType
    switch (bpType) {
    case STATIC:
        return TAKEN;
    case GSHARE:
    case TOURNAMENT:
    case CUSTOM:
        return predictor->make_prediction(pc);
    default:
        break;
    }

    // If there is not a compatable bpType then return NOTTAKEN
    return NOTTAKEN;
}

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void train_predictor(uint32_t pc, uint8_t outcome) {
    //
    // TODO: Implement Predictor training
    //

    // Train the predictor based on the bpType
    switch (bpType) {
    case STATIC:
        break;
    case GSHARE:
    case TOURNAMENT:
    case CUSTOM:
        predictor->train_predictor(pc, outcome);
        break;
    default:
        break;
    }
}

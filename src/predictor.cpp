//========================================================//
//  predictor.c                                           //
//  Source file for the Branch Predictor                  //
//                                                        //
//  Implement the various branch predictors below as      //
//  described in the README                               //
//========================================================//
#include "predictor.hpp"
#include "gshare.hpp"
#include "perceptron.hpp"
#include "tournament.hpp"

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
int threshold;    // Threshold for perceptron predictor
int bpType;       // Branch Prediction Type
int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//

//
// TODO: Add your own Branch Predictor data structures here
//
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
        predictor = new PerceptronPredictor(pcIndexBits, ghistoryBits, threshold);
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

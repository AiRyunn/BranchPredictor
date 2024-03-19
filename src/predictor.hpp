//========================================================//
//  predictor.h                                           //
//  Header file for the Branch Predictor                  //
//                                                        //
//  Includes function prototypes and global predictor     //
//  variables and defines                                 //
//========================================================//

#ifndef PREDICTOR_HPP
#define PREDICTOR_HPP

#include <stdint.h>
#include <stdlib.h>
#include <vector>

//
// Student Information
//
extern const char *studentName;
extern const char *studentID;
extern const char *email;

//------------------------------------//
//      Global Predictor Defines      //
//------------------------------------//
#define NOTTAKEN  0
#define TAKEN     1

// The Different Predictor Types
#define STATIC      0
#define GSHARE      1
#define TOURNAMENT  2
#define CUSTOM      3
extern const char *bpName[];

// Definitions for 2-bit counters
#define SN  0			// predict NT, strong not taken
#define WN  1			// predict NT, weak not taken
#define WT  2			// predict T, weak taken
#define ST  3			// predict T, strong taken

//------------------------------------//
//      Predictor Configuration       //
//------------------------------------//
extern int ghistoryBits; // Number of bits used for Global History
extern int lhistoryBits; // Number of bits used for Local History
extern int pcIndexBits;  // Number of bits used for PC index
extern int bpType;       // Branch Prediction Type
extern int verbose;

//------------------------------------//
//      Predictor Data Structures     //
//------------------------------------//
class BasePredictor {
public:
    virtual uint8_t make_prediction(uint32_t pc) = 0;
    virtual void train_predictor(uint32_t pc, uint8_t outcome) = 0;
};

class GsharePredictor : public BasePredictor {
private:
    uint32_t GHR;             // Global history record
    std::vector<uint8_t> BHT; // Branch history table
    uint32_t gHistoryBits;     // Number of bits used for history
    uint32_t indexMask;       // Mask to extract relevant bits
public:
    GsharePredictor(uint32_t gHistoryBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

class BimodelPredictor : public BasePredictor {
private:
    std::vector<uint8_t> BHT; // Branch history table
    uint32_t indexMask;       // Mask to extract relevant bits
public:
    BimodelPredictor(uint32_t lHistoryBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

class TournamentPredictor : public BasePredictor {
private:
    GsharePredictor *gshare;
    BimodelPredictor *bimodal;
    std::vector<uint8_t> chooser; // Choice table
    uint32_t indexMask;           // Mask to extract relevant bits
public:
    TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

class CustomPredictor : public BasePredictor {
public:
    CustomPredictor();
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

//------------------------------------//
//    Predictor Function Prototypes   //
//------------------------------------//

// Initialize the predictor
//
void init_predictor();

// Make a prediction for conditional branch instruction at PC 'pc'
// Returning TAKEN indicates a prediction of taken; returning NOTTAKEN
// indicates a prediction of not taken
//
uint8_t make_prediction(uint32_t pc);

// Train the predictor the last executed branch at PC 'pc' and with
// outcome 'outcome' (true indicates that the branch was taken, false
// indicates that the branch was not taken)
//
void train_predictor(uint32_t pc, uint8_t outcome);

#endif

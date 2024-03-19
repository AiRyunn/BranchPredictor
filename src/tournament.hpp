#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "predictor.hpp"

class TournamentPredictor : public BasePredictor {
private:
    BasePredictor *gPredictor;
    BasePredictor *lPredictor;
    std::vector<uint8_t> chooser; // Choice table
    uint32_t indexMask;           // Mask to extract relevant bits
public:
    TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

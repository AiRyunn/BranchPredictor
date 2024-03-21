#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "predictor.hpp"

class LocalPredictor : public BasePredictor {
private:
    std::vector<uint32_t> LPT;   // Local prediction table
    std::vector<uint8_t> BHT;    // Branch history table (2-bit saturating counter)
    const uint32_t indexMask;    // Mask of the index
    const uint32_t lHistoryMask; // Mask of the local history
public:
    LocalPredictor(uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) const override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

class GlobalPredictor : public BasePredictor {
private:
    uint32_t GHR;                // Global history register
    std::vector<uint8_t> GHT;    // Global history table (2-bit saturating counter)
    const uint32_t gHistoryMask; // Mask of the global history
public:
    GlobalPredictor(uint32_t gHistoryBits);
    uint8_t make_prediction(uint32_t pc) const override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

// Tournament predictor of Alpha 21264: https://acg.cis.upenn.edu/milom/cis501-Fall09/papers/Alpha21264.pdf
// Implementation based on: https://course.ece.cmu.edu/~ece447/s15/lib/exe/fetch.php?media=cmpwrgd.pdf
// Modifications from the original Alpha 21264 design include:
// 1. A 2-bit saturating counter is used for the choice predictor instead of a 3-bit saturating counter.
// 2. The choice predictor is indexed by the PC instead of the global history.
// Memory usage: 5*2^pcIndexBits + 2^lHistoryBits + 2^gHistoryMask bytes
class TournamentPredictor : public BasePredictor {
private:
    GlobalPredictor *gPredictor;
    LocalPredictor *lPredictor;
    uint32_t GHR;                 // Global history register
    std::vector<uint8_t> chooser; // Choice table
    const uint32_t indexMask;     // Mask of the index
public:
    TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) const override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

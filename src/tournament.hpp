#ifndef TOURNAMENT_HPP
#define TOURNAMENT_HPP

#include "predictor.hpp"

class LocalPredictor : public BasePredictor {
private:
    std::vector<uint32_t> LPT; // Local prediction table
    std::vector<uint8_t> BHT; // Branch history table (2-bit saturating counter)
    uint32_t indexMask;       // Mask to extract relevant bits
    uint32_t lHistoryMask;    // Number of bits to use as index into BHT
public:
    LocalPredictor(uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

class GlobalPredictor : public BasePredictor {
private:
    uint32_t GHR;             // Global history record
    std::vector<uint8_t> GHT; // Global history table (2-bit saturating counter)
    uint32_t gHistoryMask;    // Number of bits to use as index into GHT
public:
    GlobalPredictor(uint32_t gHistoryBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

// Tournament predictor of Alpha 21264: https://acg.cis.upenn.edu/milom/cis501-Fall09/papers/Alpha21264.pdf
// Implementation based on: https://course.ece.cmu.edu/~ece447/s15/lib/exe/fetch.php?media=cmpwrgd.pdf
// Memory usage: 4*2^pcIndexBits + 2^lHistoryBits + 2^gHistoryMask bytes
class TournamentPredictor : public BasePredictor {
private:
    LocalPredictor *lPredictor;
    GlobalPredictor *gPredictor;
    uint32_t GHR;             // Global history record
    std::vector<uint8_t> chooser; // Choice table
    uint32_t gHistoryMask;    // Number of bits to use as index into GHT
    // uint32_t indexMask;           // Mask to extract relevant bits
public:
    TournamentPredictor(uint32_t gHistoryBits, uint32_t lHistoryBits, uint32_t pcIndexBits);
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

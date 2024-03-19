#ifndef CUSTOM_HPP
#define CUSTOM_HPP

#include "predictor.hpp"

class CustomPredictor : public BasePredictor {
public:
    CustomPredictor();
    uint8_t make_prediction(uint32_t pc) override;
    void train_predictor(uint32_t pc, uint8_t outcome) override;
};

#endif

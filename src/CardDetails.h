// (c) 2017 Sam Donow
#pragma once
#include "Cards.h"

#define DEF_ACTION(Type) \
class Type : public Action {\
public: \
    static constexpr CardSpec spec; \
    static constexpr ActionID ID; \
    static constexpr bool isPermanent; \
    Type() : Action(spec, isPermanent) {} \
    bool effect(const std::vector<int> &playerChoice, Player &player) final; \
    std::vector<int> queryChoice(Player &player) final; \
    bool legal(const std::vector<int> &choices, const Player &player) final; \
} \

DEF_ACTION(Survey);
DEF_ACTION(Warfare);
DEF_ACTION(Colonize);
DEF_ACTION(ProduceTrade);
DEF_ACTION(Research);

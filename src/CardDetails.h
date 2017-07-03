// (c) 2017 Sam Donow
#pragma once
#include "Cards.h"

#define DEF_ACTION(Type) \
class Type : public Action { \
public: \
    Type(); \
    bool effect(const std::vector<int> &playerChoice, Player &player) const final; \
    std::vector<int> queryChoice(Player &player) const final; \
    bool legal(const std::vector<int> &choices, const Player &player) const final { return true; }; \
}; \

ACT_ON_ALL_ACTIONS(DEF_ACTION)

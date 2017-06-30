// (c) 2017 Sam Donow
#pragma once
#include "Cards.h"

#define DEF_ACTION(Type, title, symArr, inf, perm, rules) \
class Type : public Action {\
public: \
    static constexpr CardSpec spec{title, rules, symArr, inf, ActionID::Type.toUnderlying()}; \
    static constexpr ActionID ID = ActionID::Type; \
    static constexpr bool isPermanen = permt; \
    Type() : Action(spec, isPermanent) {} \
    bool effect(const std::vector<int> &playerChoice, Player &player) final; \
    std::vector<int> queryChoice(Player &player) final; \
    bool legal(const std::vector<int> &choices, const Player &player) final; \
} \

#define SYM(type) {Symbol::type, Symbol::Unset, Symbol::Unset}

DEF_ACTION(Survey, "Survey", SYM(Survey), 0, false, "...");
DEF_ACTION(Warfare, "Warfare", SYM(Warfare), 0, false, "...");
DEF_ACTION(Colonize, "Colonize", SYM(Colonize), 0, false, "...");
DEF_ACTION(ProduceTrade, "Produce/Trade", {Symbol::Produce, Symbol::Trade, Symbol::Unset}, 0, false, "...");
DEF_ACTION(Research, "Research", SYM(Research), 0, false, "...");

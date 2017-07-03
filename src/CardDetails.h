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

#define DEF_PLANET(Type) \
class Type : public Planet { \
public: \
    Type(); \
};
ACT_ON_ALL_ACTIONS(DEF_ACTION)
ACT_ON_ALL_PLANETS(DEF_PLANET)
#undef DEF_ACTION
#undef DEF_PLANET

// helper macros for specifying the details of cards

#define SYM_3(t1, t2, t3) std::array<Symbol, 3>{{Symbol::t1, Symbol::t2, Symbol::t3}}
#define SYM_2(t1, t2) SYM_3(t1, t2, Unset)
#define SYM_1(t1) SYM_2(t1, Unset)
#define SYM_0() SYM_1(Unset)

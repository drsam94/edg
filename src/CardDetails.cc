// (c) 2017 Sam Donow
#include "CardDetails.h"
#include "Player.h"
#include<vector>
#define DETAILS(Type, title, symArr, inf, perm, rules) \
Type::Type() : Action(CardSpec{title, rules, symArr, inf, ActionID::Type}, \
            ActionID::Type, \
            perm) {} \

#define SYM_3(t1, t2, t3) std::array<Symbol, 3>{{Symbol::t1, Symbol::t2, Symbol::t3}}
#define SYM_2(t1, t2) SYM_3(t1, t2, Unset)
#define SYM_1(t1) SYM_2(t1, Unset)
#define SYM_0() SYM_1(Unset)

DETAILS(Survey, "Survey", SYM_1(Survey), 0, false, "...")
DETAILS(Warfare, "Warfare", SYM_1(Warfare), 0, false, "...")
DETAILS(Colonize, "Colonize", SYM_1(Colonize), 0, false, "...")
DETAILS(ProduceTrade, "Produce/Trade", SYM_2(Symbol::Produce, Symbol::Trade),
        0, false, "...")
DETAILS(Research, "Research", SYM_1(Research), 0, false, "...")
DETAILS(Politics, "Politics", SYM_0(), 0, false, "...")

#define EFFECT(CardType) \
bool CardType::effect(const std::vector<int> &playerChoice, Player &player) const

EFFECT(Survey) {
    player.drawCards(2);
    return true;
}

EFFECT(Warfare) {
    if (playerChoice[0] == 0) {
        player.getFighters(0);
    } else {
        player.attackPlanet(playerChoice[1]);
    }
    return true;
}

EFFECT(Colonize) {
    if (playerChoice[0] == 0) {
        player.settlePlanet(playerChoice[1]);
        return true;
    } else {
        player.addColony(playerChoice[1], getID());
        return false;
    }
}

EFFECT(ProduceTrade) {
    if (playerChoice[0] == 0) {
        player.produce(playerChoice[1], playerChoice[2]);
    } else {
        player.trade(playerChoice[1], playerChoice[2]);
    }
    return true;
}

EFFECT(Research) {
    for (int handIndex : playerChoice) {
        player.removeFromHand(handIndex);
    }
    return true;
}

EFFECT(Politics) {
    player.gainRole(playerChoice[0]);
    return false;
}

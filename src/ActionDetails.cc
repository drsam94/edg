// (c) 2017 Sam Donow
#include "CardDetails.h"
#include "Player.h"
#include<vector>
#define DETAILS(Type, title, symArr, inf, perm, rules) \
Type::Type() : Action(CardSpec{title, rules, symArr, inf, ActionID::Type}, \
            perm) {} \

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
    player.gainRoleTo(static_cast<Role>(playerChoice[0]), true);
    return false;
}

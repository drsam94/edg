// (c) 2017 Sam Donow
#include "CardDetails.h"
#include "Player.h"
#include "State.h"
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

// These Macros aren't just to save me space, I may want to change this fcn signature,
// and this will make it easier, I hope
#define EFFECT(CardType) \
bool CardType::effect(const std::vector<int> &playerChoice, Player &player) const

#define QUERY(CardType) \
std::vector<int> CardType::queryChoice(Player &player) const

QUERY(Survey) {
    return {};
}

EFFECT(Survey) {
    player.drawCards(2);
    return true;
}

QUERY(Warfare) {
    return player.adapter.composedAlternatives("get fighter",
            &ChoiceAdapter::nullChoice, std::make_tuple(),
            "attack planet", &ChoiceAdapter::chooseOneOfFDPlanets, std::forward_as_tuple(player.state->planets));
}

EFFECT(Warfare) {
    if (playerChoice[0] == 0) {
        player.getFighters(0);
    } else {
        player.attackPlanet(playerChoice[1]);
    }
    return true;
}

QUERY(Colonize) {
    return player.adapter.composedAlternatives("Add a colony",
            &ChoiceAdapter::chooseOneOfFDPlanets, std::forward_as_tuple(player.state->planets),
            "Settle a planet",
            &ChoiceAdapter::chooseOneOfFDPlanets, std::forward_as_tuple(player.state->planets));
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

QUERY(ProduceTrade) {
    return player.adapter.composedAlternatives("Produce a resource",
            &ChoiceAdapter::chooseResourceSlots,
            // TODO: pretty sure this causes a copy of planets, be careful
            std::make_tuple(1, player.state->planets, true),
            "Trade a resource", &ChoiceAdapter::chooseResourceSlots,
            std::make_tuple(1, player.state->planets, false));
}
EFFECT(ProduceTrade) {
    if (playerChoice[0] == 0) {
        player.produce(playerChoice[1], playerChoice[2]);
    } else {
        player.trade(playerChoice[1], playerChoice[2]);
    }
    return true;
}

// Extract "choose multiple cards in hand" logic from dissent and stuff
QUERY(Research) {
    return {};
}
EFFECT(Research) {
    for (int handIndex : playerChoice) {
        player.removeFromHand(handIndex);
    }
    return true;
}

QUERY(Politics) {
    return {};
}

EFFECT(Politics) {
    player.gainRoleTo(static_cast<Role>(playerChoice[0]), true);
    return false;
}

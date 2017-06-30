// (c) 2017 Sam Donow
#include "CardDetails.h"

#define EFFECT(CardType) \
bool CardType::effect(const std::vector<int> &playerChoice, Player &player)

EFFECT(Survey) {
    player.drawCards(2);
    return true;
}

Effect(Warfare) {
    if (playerChoice[0] == 0) {
        player.getFighter();
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
        player.addColony(playerChoice[1], this);
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

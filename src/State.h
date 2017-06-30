// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "Cards.h"
#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <random>

class PlanetState {
    Planet card;
    bool revealed;
    bool scorched;
    std::vector<ActionID> colonies;
    std::array<bool, 3> resourcesFilled;

    void flip() { revealed = true; }

};

class PlayerState {
    std::vector<ActionID> hand;
    std::vector<ActionID> deck;
    std::vector<ActionID> discard;
    std::vector<ActionID> inPlay;
    std::vector<ActionID> permanents;
    std::vector<PlanetState> planets;
    uint8_t fighters;
    uint8_t influenceTokens;

  public:
    void draw(int cards);
};

class RoleState {
    // need to be careful about Produce/Trade special case
    std::unordered_map<Role, uint8_t> roleCards;
};

class TechTree {
    // Gets more complex in the expansion
    struct TechCost {
        PlanetType type;
        uint8_t amount;
    };
    std::map<TechCost, ActionID> cards;
};

class GameState {
    std::vector<PlayerState> players;
    RoleState roles;
    std::deque<Planet> planetDeck;
    TechTree availableTechs;
    uint8_t unclaimedInfluence;
    uint8_t unclaimedBlueInfluence;
    std::unordered_map<Resource, uint8_t> unclaimedResources;
  public:
    static std::mt19937 rng;

    Player &getNextPlayer();
};



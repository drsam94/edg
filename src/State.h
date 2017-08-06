// (c) 2017 Sam Donow
#pragma once
#include "Cards.h"
#include "CoreEnums.h"
#include "GodBook.h"
#include <vector>
#include <deque>
#include <unordered_map>
#include <map>
#include <random>

struct PlanetState {
    PlanetID card;
    bool revealed;
    bool scorched;
    std::vector<ActionID> colonies;
    std::array<bool, 3> resourcesFilled;

    void flip() { revealed = true; }
    PlanetState(PlanetID id) : card(id), revealed(false),
        scorched(false), colonies{}, resourcesFilled{{ false, false, false }} {}
    const Planet &getCard() const {
        return GodBook::instance().getPlanet(card);
    }

    friend std::ostream &operator<<(std::ostream &os, const PlanetState &state);
};

struct PlayerState {
    std::string name;
    std::vector<ActionID> hand;
    std::vector<ActionID> deck;
    std::vector<ActionID> discard;
    std::vector<ActionID> inPlay;
    std::vector<ActionID> permanents;
    std::vector<PlanetState> planets;
    uint8_t fighters;
    uint8_t influence;

    void draw(int cards);
    size_t staticSymCount(Symbol sym) const;

    std::ostream &print(std::ostream &os, bool showHand) const;
    friend std::ostream &operator<<(std::ostream &os, const PlayerState &state);
};

class RoleState {
    // need to be careful about Produce/Trade special case
    std::unordered_map<ActionID, uint8_t> roleCards;
  public:
    bool removeRole(Role role) {
        uint8_t &amount = roleCards[RoleToAction(role)];
        if (amount == 0) {
            return false;
        } else {
            --amount;
            return true;
        }
    }
    int count(Role role) const {
        return static_cast<int>(roleCards.find(RoleToAction(role))->second);
    }
    void init(int numPlayers);

    friend std::ostream &operator<<(std::ostream &os, const RoleState &state);
};

class TechTree {
    // Gets more complex in the expansion
    struct TechCost {
        PlanetType type;
        uint8_t amount;
    };
    std::multimap<TechCost, ActionID> cards;
  public:
    // Maybe we should use like a bimap or something, idk
    void erase(ActionID id) {
        for (auto it = cards.begin(); it != cards.end(); ++it) {
            if (it->second == id) {
                cards.erase(it);
                return;
            }
        }
    }
    void init();

    friend std::ostream &operator<<(std::ostream &os, const TechTree &state);
};

struct GameState {
    std::vector<PlayerState> players;
    RoleState roles;
    std::deque<PlanetID> planetDeck;
    TechTree availableTechs;
    uint8_t unclaimedInfluence;
    uint8_t currentPlayerIndex;
    static std::mt19937 rng;

    bool endCondition() const { return false; }
    void init(int numPlayers);
    size_t numPlayers() const { return players.size(); }

    friend std::ostream &operator<<(std::ostream &os, const GameState &state);
};

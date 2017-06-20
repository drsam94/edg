// (c) Sam Donow 2017
#pragma once
#include "CoreEnums.h"

class GameState;
class PlayerState;
class ChoiceAdapter;
// Raw information needed for all cards
struct CardSpec {
    std::string title;
    std::string displayText;
    std::array<Symbol, 3> symbols;
    short influence;
    short id;
};

// Class that represents any physical card
// (Actions, Permanents, Planets)
class Card : private CardSpec {
  public:
    explicit Card(const CardSpec &spec) : CardSpec(spec) {}

    const std::string &name() const { return title; }
    const std::string &text() const { return displayText; }
    short points() const { return influence; }
    bool operator==(const Card &other) { return id == other.id; }
}

// TODO: pass around just IDs instead of actual Action objects?
class Action : public Card {
  private:
    const bool isPermanent;
  public:
    Action(const CardSpec &spec, bool permanent) :
        CardSpec(spec), isPermanent(permanent) {}
    virtual std::vector<int> queryChoice(Player &player) { return {}; }
    virtual bool legal(const std::vector<int> &choices, const Player &player) { return true; }
    // Returns true if the card should be moved to the discard
    // returns false otherwise (maybe moved to a colony, maybe removed from game)
    virtual bool effect(const std::vector<int> &playerChoice, Player &player) = 0;
};

class Planet : public Card {
  private:
    PlanetType type;
    uint8_t colonyCost;
    uint8_t fighterCost;
    bool handSizePlus;
    std::array<Resource, 3> resourceSpots;
}


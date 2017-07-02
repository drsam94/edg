// (c) Sam Donow 2017
#pragma once
#include "CoreEnums.h"
#include "Player.h"
#include <vector>
#include <array>
#include <string>

// Raw information needed for all cards
struct CardSpec {
    std::string title;
    std::string displayText;
    std::array<Symbol, 3> symbols;
    short influence;
    int uniqid;
};

// Class that represents any physical card
// (Actions, Permanents, Planets)
struct Card : public CardSpec {
    explicit Card(const CardSpec &spec) : CardSpec(spec) {}

    const std::string &name() const { return title; }
    const std::string &text() const { return displayText; }
    short points() const { return influence; }
    bool operator==(const Card &other) { return uniqid == other.uniqid; }
};

struct Action : public Card {
    const ActionID ID;
    const bool isPermanent;
    Action(const CardSpec &spec, ActionID id, bool permanent) :
        Card(spec), ID(id), isPermanent(permanent) {}
    virtual std::vector<int> queryChoice(Player &player) const { return {}; }
    virtual bool legal(const std::vector<int> &choices, const Player &player) const { return true; }
    // Returns true if the card should be moved to the discard
    // returns false otherwise (maybe moved to a colony, maybe removed from game)
    virtual bool effect(const std::vector<int> &playerChoice, Player &player) const = 0;
    ActionID getID() const { return ID; }
};

struct Planet : public Card {
    PlanetType type;
    uint8_t colonyCost;
    uint8_t fighterCost;
    bool handSizePlus;
    std::array<Resource, 3> resourceSpots;
};

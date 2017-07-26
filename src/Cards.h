// (c) Sam Donow 2017
#pragma once
#include "CoreEnums.h"
#include <vector>
#include <array>
#include <string>
#include <iostream>

class Player;
// Raw information needed for all cards
struct CardSpec {
    const std::string title;
    const std::string displayText;
    // hmm, probably could be changed into a bitset
    const std::array<Symbol, 3> symbols;
    const short influence;
    const int uniqid;

    friend std::ostream &operator<<(std::ostream &os, const CardSpec &spec);
};

// Class that represents any physical card
// (Actions, Permanents, Planets)
struct Card : public CardSpec {
    explicit Card(const CardSpec &spec) : CardSpec(spec) {}

    const std::string &name() const { return title; }
    const std::string &text() const { return displayText; }
    short points() const { return influence; }
    bool operator==(const Card &other) { return uniqid == other.uniqid; }
    friend std::ostream &operator<<(std::ostream &os, const Card &card) { return os << static_cast<const CardSpec &>(card); }
    size_t countSyms(Symbol sym) const { return std::count(symbols.begin(), symbols.end(), sym); }
};

struct Action : public Card {
    const bool isPermanent;
    Action(const CardSpec &spec, bool permanent) :
        Card(spec), isPermanent(permanent) {}
    virtual std::vector<int> queryChoice(Player &player) const { return {}; }
    virtual bool legal(const std::vector<int> &choices, const Player &player) const { return true; }
    // Returns true if the card should be moved to the discard
    // returns false otherwise (maybe moved to a colony, maybe removed from game)
    virtual bool effect(const std::vector<int> &playerChoice, Player &player) const = 0;
    ActionID getID() const { return static_cast<ActionID>(uniqid); }
    friend std::ostream &operator<<(std::ostream &os, const Action &action) { return os << static_cast<const CardSpec &>(action); }
};

struct Planet : public Card {
    const PlanetType type;
    const int8_t colonyCost;
    const int8_t fighterCost;
    const std::array<Resource, 3> resourceSpots;
    static const inline int PlanetIDOffset = 1000;

    Planet(const CardSpec &spec, PlanetType pTy, int8_t colonies, int8_t fighters,
            const std::array<Resource, 3> rs) :
        Card(spec), type(pTy), colonyCost(colonies), fighterCost(fighters),
        resourceSpots(rs) {}
    PlanetID getID() const { return static_cast<PlanetID>(uniqid - PlanetIDOffset); }
    size_t numResourceSlots() const { return std::find(std::cbegin(resourceSpots), std::cend(resourceSpots), Resource::Unset) - std::cbegin(resourceSpots);}
    friend std::ostream &operator<<(std::ostream &os, const Planet &planet) { return os << static_cast<const CardSpec &>(planet); }
};

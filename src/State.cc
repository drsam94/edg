#include "State.h"
#include<random>

std::mt19937 GameState::rng{std::random_device{}()};

inline const std::array<ActionID, 10> StartingHand = {{
    ActionID::Survey, ActionID::Survey, ActionID::Warfare, ActionID::Colonize, ActionID::Colonize,
    ActionID::ProduceTrade, ActionID::ProduceTrade, ActionID::Research, ActionID::Research, ActionID::Politics
}};

void PlayerState::draw(int cards) {
    do {
        while (!deck.empty() && cards-- > 0) {
            hand.push_back(deck.back());
            deck.pop_back();
        }
        if (deck.empty()) {
            std::shuffle(discard.begin(), discard.end(), GameState::rng);
            deck.swap(discard);
        }
    } while (cards && !deck.empty());
}

void RoleState::init(int numPlayers) {
    // TODO 3p extended?
    roleCards[Role::Survey] = 20 - 2 * numPlayers;
    roleCards[Role::Warfare] = 16 - numPlayers;
    roleCards[Role::Colonize] = 20 - 2 * numPlayers;
    // TODO decide if we should change the key or just wrap it with a sane API
    roleCards[Role::Produce] = 18 - 2 * numPlayers;
    roleCards[Role::Research] = 20 - 2 * numPlayers;
}

void GameState::init(int numPlayers) {
    roles.init(numPlayers);
    for (int i = 0; i < numPlayers; ++i) {
        players.emplace_back();
        auto &deck = players.back().deck;
        deck.insert(deck.begin(), StartingHand.begin(), StartingHand.end());
        std::shuffle(deck.begin(), deck.end(), GameState::rng);
        players.back().draw(5);
    }
    std::vector<PlanetID> startingPlanets = {{
        PlanetID::StartSaturn1, PlanetID::StartSaturn2, PlanetID::StartEarth1,
        PlanetID::StartEarth2, PlanetID::StartMars1, PlanetID::StartMars2
    }};
    std::shuffle(startingPlanets.begin(), startingPlanets.end(), GameState::rng);
    for (PlayerState &player : players) {
        player.planets.emplace_back(startingPlanets.back());
        startingPlanets.end();
    }
}

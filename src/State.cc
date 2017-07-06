#include "State.h"
#include<random>

std::mt19937 GameState::rng{std::random_device{}()};

void PlayerState::draw(int cards) {
    if (cards <= 0) {
        return;
    }
    do {
        while (!deck.empty() && cards > 0) {
            hand.push_back(deck.back());
            deck.pop_back();
            --cards;
        }
        if (cards > 0 && deck.empty()) {
            std::shuffle(discard.begin(), discard.end(), GameState::rng);
            deck.swap(discard);
        }
    } while (cards > 0 && !deck.empty());
}

size_t PlayerState::staticSymCount(Symbol sym) const {
    size_t ret = 0;
    for (const PlanetState &planet : planets) {
        if (planet.revealed) {
            ret += planet.getCard().countSyms(sym);
        }
    }
    for (const ActionID &perm : permanents) {
        ret += GodBook::instance().getAction(perm).countSyms(sym);
    }
    return ret;
}

void RoleState::init(int numPlayers) {
    // TODO 3p extended?
    roleCards[ActionID::Survey] = 20 - 2 * numPlayers;
    roleCards[ActionID::Warfare] = 16 - numPlayers;
    roleCards[ActionID::Colonize] = 20 - 2 * numPlayers;
    // TODO decide if we should change the key or just wrap it with a sane API
    roleCards[ActionID::ProduceTrade] = 18 - 2 * numPlayers;
    roleCards[ActionID::Research] = 20 - 2 * numPlayers;
}

inline const std::array<ActionID, 10> StartingHand = {{
    ActionID::Survey, ActionID::Survey, ActionID::Warfare, ActionID::Colonize, ActionID::Colonize,
    ActionID::ProduceTrade, ActionID::ProduceTrade, ActionID::Research, ActionID::Research, ActionID::Politics
}};

void GameState::init(int numPlayers) {
    if (!players.empty()) {
        throw std::runtime_error("Trying to reinit gamestate");
    }
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
    currentPlayerIndex = 0;
}

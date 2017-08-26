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

// toString() methods

std::ostream &operator<<(std::ostream &os, const PlanetState &state) {
    const Planet &planet = state.getCard();
    if (!state.revealed) {
        os << "(FD) " << planet.type << " Costs: " << static_cast<int>(planet.colonyCost) << "(c) "
           << static_cast<int>(planet.fighterCost) << "(f) colonies: " << state.colonies.size();
    } else {
        os << planet << (state.scorched ? " (scorched) " : "") << " res: ";
        for (size_t i = 0; i < planet.numResourceSlots(); ++i) {
            os << (state.resourcesFilled[i] ? planet.resourceSpots[i].str() + " " : "");
        }
    }
    return os;
}

std::ostream &PlayerState::print(std::ostream &os, bool showHand) const {
    os << "Player " << name << "'s hand:";
    if (showHand) {
        os << "\n";
        for (ActionID id : hand) {
            os << "  " << GodBook::instance().getAction(id) << "\n";
        }
    } else {
        os << " " << hand.size() << " ";
    }
    os << "deck: " << deck.size() << " discard: " << discard.size();
    // TODO print permanents/inPlay
    os << "\nplanets:\n";
    for (const PlanetState &pState : planets) {
        os << "  " << pState << "\n";
    }
    os << "fighters: " << static_cast<int>(fighters)
       << " influence: " << static_cast<int>(influence);
    return os;
}

std::ostream &operator<<(std::ostream &os, const PlayerState &state) {
    return state.print(os, true);
}

std::ostream &operator<<(std::ostream &os, const RoleState &state) {
    for (Role role : Role::values()) {
        if (role == Role::Produce) continue;
        os << RoleToAction(role) << ": " << state.count(role) << "\n";
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const TechTree &tree) {
    return os << " UNIMPLEMENTED\n";
}

std::ostream &operator<<(std::ostream &os, const GameState &state) {
    os << "players:\n";
    for (const PlayerState &pState : state.players) {
        pState.print(os, false);
        os << "\n";
    }

    os << "roles:\n" << state.roles;
    // TODO Print the top card of the Planet deck, but only FD
    os << "techs available:\n" << state.availableTechs;
    os << "influence available: " << static_cast<int>(state.unclaimedInfluence);
    return os;
}

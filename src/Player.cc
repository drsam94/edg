#include "Player.h"
#include "State.h"
#include "GodBook.h"

ActionID Player::getActionChoice() {
    int actionChoice = adapter.chooseAction();
    if (actionChoice < 0) {
        return ActionID::Unset;
    } else {
        // erase the thing from the hand
        return state->hand[actionChoice];
    }
}

bool Player::playAction(ActionID id) {
    const Action &action = GodBook::instance().getAction(id);
    std::vector<int> choices = action.queryChoice(*this);
    if (!action.legal(choices, *this))
        return false;
    state->hand.erase(find(begin(state->hand), end(state->hand), action.getID()));
    if (action.effect(choices, *this)) {
        state->inPlay.emplace_back(action.getID());
    }
    return true;
}

Role Player::chooseRole() {
    return Role::Survey; // use adapter;
}

void Player::drawCards(int count) {
    state->draw(count);
}

void Player::getFighters(int count) {
    state->fighters += count;
}

void Player::settlePlanet(int planetIdx) {
    PlanetState &pstate = state->planets[planetIdx];
    pstate.flip();
    state->discard.insert(state->discard.end(), pstate.colonies.begin(), pstate.colonies.end());
    pstate.colonies.clear();
}

void Player::attackPlanet(int planetIdx) {
    PlanetState &pstate = state->planets[planetIdx];
    pstate.flip();
    state->fighters -= pstate.getCard().fighterCost;
}
void Player::addColony(int planetIdx, ActionID action) {
    state->planets[planetIdx].colonies.push_back(action);
}

void Player::produce(int planetIdx, int resourceSlotIdx) {
    state->planets[planetIdx].resourcesFilled[resourceSlotIdx] = true;
}

void Player::trade(int planetIdx, int resourceSlotIdx) {
    state->planets[planetIdx].resourcesFilled[resourceSlotIdx] = false;
    ++state->influence;
}

ActionID Player::removeFromHand(int handIdx) {
    auto it = state->hand.begin() + handIdx;
    state->hand.erase(it);
    return *it;
}

ActionID Player::gainRoleTo(Role role, bool toHand) {
    const bool getCard = gameState->roles.removeRole(role);
    if (getCard) {
        const ActionID action = RoleToAction(role);
        if (toHand) {
            state->hand.push_back(action);
        }
        return action;
    }
    return ActionID::Unset;
}

void Player::doRole(Role role, bool leader) {
    std::vector<int> choices = adapter.getDissentBoostFollowChoice(role, leader);
    if (choices[0] == 0) {
        // dissent
        drawCards(1);
        return;
    }
    std::vector<ActionID> cardsBeingUsed;
    ActionID roleCardFromCenter = gainRoleTo(role, false);
    if (roleCardFromCenter.valid()) {
        cardsBeingUsed.push_back(roleCardFromCenter);
    }
    for (size_t i = 2; i < choices.size(); ++i) {
        ActionID id = removeFromHand(choices[i]);
        cardsBeingUsed.push_back(id);
    }
    size_t symcount = 0;
    for (ActionID action : cardsBeingUsed) {
        symcount += GodBook::instance().getAction(action).countSyms(RoleToSymbol(role));
    }

    switch (role) {
        case Role::Survey: {
            std::vector<PlanetID> planetsToSee;
            const size_t numToLook = std::max(gameState->planetDeck.size(),
                    static_cast<size_t>(symcount + leader - 1));
            for (size_t i = 0; i < numToLook; ++i) {
                planetsToSee.push_back(gameState->planetDeck.back());
                gameState->planetDeck.pop_back();
            }
            const size_t planetChoice = adapter.chooseOneOfPlanetCards(planetsToSee);
            for (size_t i = 0; i < planetsToSee.size(); ++i) {
                if (i == planetChoice) {
                    state->planets.emplace_back(planetsToSee[i]);
                }
                gameState->planetDeck.push_front(planetsToSee[i]);
            }
            break;
        }
        case Role::Warfare: {
            if (leader && choices[2] == 0) {
                // attack a planet, symbols don't matter
                const size_t planet = adapter.chooseOneOfFDPlanets(state->planets);
                attackPlanet(planet);
            } else {
                getFighters(symcount);
            }
            break;
        }
        case Role::Colonize: {
            if (leader && choices[2] == 0) {
                const size_t planet = adapter.chooseOneOfFDPlanets(state->planets);
                settlePlanet(planet);
            } else {
                std::vector<int> planetPlacements = adapter.placeColonies(cardsBeingUsed, state->planets);
                for (size_t i = 0; i < planetPlacements.size(); ++i) {
                    addColony(planetPlacements[i], cardsBeingUsed[i]);
                }
                cardsBeingUsed.clear();
                // this is complex
            }
            break;
        }
        case Role::Produce: {
            std::vector<int> producePlaces = adapter.chooseResourceSlots(symcount, state->planets, true);
            for (size_t i = 0; i < producePlaces.size(); i += 2) {
                produce(producePlaces[i], producePlaces[i + 1]);
            }
            break;
        }
        case Role::Trade: {
            std::vector<int> tradePlaces = adapter.chooseResourceSlots(symcount, state->planets, false);
            for (size_t i = 0; i < tradePlaces.size(); i += 2) {
                trade(tradePlaces[i], tradePlaces[i + 1]);
            }
            break;
        }
        case Role::Research: {
            ActionID action = adapter.getResearchChoice(symcount, gameState->availableTechs, state->planets);
            gameState->availableTechs.erase(action);
            state->discard.push_back(action);
            break;
        }
        default: break;
    }
    for (ActionID id : cardsBeingUsed) {
        state->discard.push_back(id);
    }
}

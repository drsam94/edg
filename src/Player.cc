#include "Player.h"
#include "State.h"
#include "GodBook.h"

ActionID Player::getActionChoice() {
    ActionID actionChoice = adapter->chooseAction(state.hand);
    if (!actionChoice.valid()) {
        return actionChoice;
    } else {
        // erase the thing from the hand
        state.hand.erase(std::find(state.hand.begin(), state.hand.end(), actionChoice));
        return actionChoice;
    }
}

bool Player::playAction(ActionID id) {
    const Action &action = GodBook::instance().getAction(id);
    std::vector<int> choices = action.queryChoice(*this);
    --actionsLeft;
    if (!action.legal(choices, *this))
        return false;
    state.hand.erase(find(begin(state.hand), end(state.hand), action.getID()));
    if (action.effect(choices, *this)) {
        state.inPlay.emplace_back(action.getID());
    }
    return true;
}

Role Player::chooseRole() {
    return static_cast<Role>(adapter->chooseRole(gameState.roles, 1)[0]);
}

void Player::drawCards(int count) {
    state.draw(count);
}

void Player::getFighters(int count) {
    state.fighters += count;
}

void Player::settlePlanet(int planetIdx) {
    PlanetState &pstate = state.planets[planetIdx];
    pstate.flip();
    state.discard.insert(state.discard.end(), pstate.colonies.begin(), pstate.colonies.end());
    pstate.colonies.clear();
}

void Player::attackPlanet(int planetIdx) {
    PlanetState &pstate = state.planets[planetIdx];
    pstate.flip();
    state.fighters -= pstate.getCard().fighterCost;
}
void Player::addColony(int planetIdx, ActionID action) {
    state.planets[planetIdx].colonies.push_back(action);
}

void Player::produce(int planetIdx, int resourceSlotIdx) {
    state.planets[planetIdx].resourcesFilled[resourceSlotIdx] = true;
}

void Player::trade(int planetIdx, int resourceSlotIdx) {
    state.planets[planetIdx].resourcesFilled[resourceSlotIdx] = false;
    ++state.influence;
}

// I feel like there may be a std::algorithm for this...
std::vector<ActionID> Player::removeFromHand(std::vector<int> handIdx) {
    std::vector<ActionID> ret;
    size_t idxIdx = 0;
    std::sort(handIdx.begin(), handIdx.end());
    for (auto it = state.hand.begin(); it != state.hand.end() && idxIdx < handIdx.size();) {
        if (it == state.hand.begin() + handIdx[idxIdx] - idxIdx) {
            ret.push_back(*it);
            it = state.hand.erase(it);
            ++idxIdx;
        } else {
            ++it;
        }
    }
    return ret;
}

ActionID Player::gainRoleTo(Role role, bool toHand) {
    const bool getCard = gameState.roles.removeRole(role);
    if (getCard) {
        const ActionID action = RoleToAction(role);
        if (toHand) {
            state.hand.push_back(action);
        }
        return action;
    }
    return ActionID::Unset;
}

void Player::doRole(Role role, bool leader) {
    std::vector<int> choices = adapter->getDissentBoostFollowChoice(role, leader, state.hand);
    if (!leader) {
        if (choices[0] == 0) {
            // dissent
            drawCards(1);
            return;
        }
        // We should really consider using something like an array view to be able to freely
        // pass in a slice here easily
        choices.erase(choices.begin());
    }
    std::vector<ActionID> cardsBeingUsed = removeFromHand(choices);
    if (leader) {
        ActionID roleCardFromCenter = gainRoleTo(role, false);
        if (roleCardFromCenter.valid()) {
            cardsBeingUsed.push_back(roleCardFromCenter);
        }
    }

    const Symbol sym = RoleToSymbol(role);
    size_t symcount = state.staticSymCount(sym);
    for (ActionID action : cardsBeingUsed) {
        symcount += GodBook::instance().getAction(action).countSyms(sym);
    }

    // TODO: second batch of choices needed for these roles
    switch (role) {
        case Role::Survey: {
            std::vector<PlanetID> planetsToSee;
            const size_t numToLook = std::max(gameState.planetDeck.size(),
                    static_cast<size_t>(symcount + leader - 1));
            for (size_t i = 0; i < numToLook; ++i) {
                planetsToSee.push_back(gameState.planetDeck.back());
                gameState.planetDeck.pop_back();
            }
            const size_t planetChoice = adapter->chooseOneOfPlanetCards(planetsToSee)[0];
            for (size_t i = 0; i < planetsToSee.size(); ++i) {
                if (i == planetChoice) {
                    state.planets.emplace_back(planetsToSee[i]);
                }
                gameState.planetDeck.push_front(planetsToSee[i]);
            }
            break;
        }
        case Role::Warfare: {
            if (leader) {
                std::vector<int> rchoices = adapter->composedAlternatives("Attack Planet",
                    &ChoiceAdapter::chooseOneOfFDPlanets,
                    std::make_tuple(std::cref(state.planets)),
                    "Get Fighters", &ChoiceAdapter::nullChoice, std::make_tuple());
                if (rchoices[0] == 0) {
                    // attack a planet, symbols don't matter
                    attackPlanet(rchoices[1]);
                } else {
                    getFighters(symcount);
                }
            } else {
                getFighters(symcount);
            }
            break;
        }
        case Role::Colonize: {
            std::vector<int> rchoices;
            if (leader) {
                rchoices = adapter->composedAlternatives("Settle a Planet",
                    &ChoiceAdapter::chooseOneOfFDPlanets,
                    std::make_tuple(std::cref(state.planets)),
                    "Place Colonies", &ChoiceAdapter::placeColonies,
                    std::make_tuple(std::cref(cardsBeingUsed), std::cref(state.planets)));
                if (rchoices[0] == 0) {
                    settlePlanet(choices[1]);
                    break;
                }
            } else {
                rchoices = adapter->placeColonies(cardsBeingUsed, state.planets);
            }
            for (size_t i = leader; i < rchoices.size(); ++i) {
                addColony(rchoices[i], cardsBeingUsed[i - leader]);
            }
            cardsBeingUsed.clear();
            break;
        }
        case Role::Produce: {
            std::vector<int> producePlaces = adapter->chooseResourceSlots(symcount, state.planets, true);
            for (size_t i = 0; i < producePlaces.size(); i += 2) {
                produce(producePlaces[i], producePlaces[i + 1]);
            }
            break;
        }
        case Role::Trade: {
            std::vector<int> tradePlaces = adapter->chooseResourceSlots(symcount, state.planets, false);
            for (size_t i = 0; i < tradePlaces.size(); i += 2) {
                trade(tradePlaces[i], tradePlaces[i + 1]);
            }
            break;
        }
        case Role::Research: {
            ActionID action = adapter->getResearchChoice(symcount, gameState.availableTechs, state.planets);
            gameState.availableTechs.erase(action);
            state.discard.push_back(action);
            break;
        }
        default: break;
    }
    for (ActionID id : cardsBeingUsed) {
        state.discard.push_back(id);
    }
}

void Player::cleanupPhase() {
    std::vector<int> cardsToDiscard = adapter->chooseCardsFromHand(state.hand, -1);
    std::vector<ActionID> cardsDiscarded = removeFromHand(cardsToDiscard);
    state.discard.insert(state.discard.end(), cardsDiscarded.begin(), cardsDiscarded.end());
    int maxHandSize = 5 + state.staticSymCount(Symbol::HandSizePlus);
    drawCards(maxHandSize - state.hand.size());
}

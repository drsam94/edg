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
    state->fighters -= pstate.card.fighterCost;
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

void Player::removeFromHand(int handIdx) {
    state->hand.erase(state->hand.begin() + handIdx);
}

void Player::gainRoleTo(Role role, bool toHand) {
    const bool getCard = gameState->roles.removeRole(role);
    if (getCard) {
        const ActionID action = RoleToAction(role);
        if (toHand) {
            state->hand.push_back(action);
        } else {
            state->discard.push_back(action);
        }
    }
}

void Player::doRole(Role role, bool leader) {
    vector<int> choices = adapter.getDissentBoostFollowChoice(role, leader);
    if (choices[0] == 0) {
        // dissent
        drawCards(1);
        return;
    }
    int symcount = choices[1];

    switch (role) {
        case Role::Survey: {
            vector<Planet> planetsToSee;

            for (int i = 0; i < symcount; ++i) {

            }
        }
    }

}

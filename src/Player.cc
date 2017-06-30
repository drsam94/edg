#include "Player.h"
std::optional<Action> Player::getActionChoice() {
    int actionChoice = adapter.chooseAction();
    if (actionChoice < 0) {
        return std::nullopt;
    } else {
        // erase the thing from the hand
        return state->hand[actionChoice];
    }
}

bool Player::playAction(const Action &action) {
    std::vector<int> choices = action.queryChoices(*this);
    if (!action.legal(choices, *this))
        return false;
    state->hand.erase(find(begin(state->hand), end(state->hand), action.ID));
    if (action.effect(choices, *this)) {
        state->inPlay.push_back(action.ID);
    }
}

void Player::leadRole(Role role) {
    std::vector<int> choices = gameState->roleState.queryChoices(role);
    gameState->roleState.doRole(role, choices, true);
}

void Player::followOrDissentRole(Role role) {
    std::vector<int> choices = gameState->roleState.queryChoices(role, true);
    if (choices[0] == 0) {
        drawCards(1);
    } else {
        gameState->roleState.doRole(role, choices, false);
    }
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

void Player::addColony(int planetIdx, ActionID action) {
    state->planets[planetIdx].colonies.push_back(action);
}

void Player::produce(int planetIdx, int resourceSlotIdx) {
    state->planets[planetIdx].resourcesFilled[resourcesSlotIdx] = true;
}

void Player::trade(int planetIdx, int resourceSlotIdx) {
    state->planets[planetIdx].resourcesFilled[resourcesSlotIdx] = false;
    ++state->influence;
}

void Player::removeFromHand(int handIdx) {
    state->hand.erase(state->hand.begin() + handIdx);
}

void Player::gainRole(int roleIdx) {
    const Role = (Role)roleIdx;
    state->hand.push_back(RoleIDToActionID(roleIdx));
    gameState.roles.removeRole(roleIdx);
}

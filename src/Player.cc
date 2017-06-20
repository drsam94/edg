#include "Player.h"
std::optional<Action> Player::getActionChoice()
{
    int actionChoice = adapter.chooseAction();
    if (actionChoice < 0) {
        return std::nullopt;
    } else {
        // erase the thing from the hand
        return state->hand[actionChoice];
    }
}

bool Player::playAction(const Action &action)
{
    std::vector<int> choices = action.queryChoices(*this);
    if (!action.legal(choices, *this))
        return false;
    state->hand.erase(find(begin(state->hand), end(state->hand), action.ID));
    if (action.effect(choices, *this)) {
        state->inPlay.push_back(action.ID);
    }
}

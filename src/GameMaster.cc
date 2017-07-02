// (c) 2017 Sam Donow
#include "GameMaster.h"

void GameMaster::gameLoop()
{
    gameState.init(4);
    do {
        Player &currentPlayer = getCurrentPlayer();
        currentPlayer.startTurn();
        while (currentPlayer.hasActionsToPlay()) {
            ActionID action = currentPlayer.getActionChoice();
            if (!action.valid()) break;

            currentPlayer.playAction(action);
        }
        Role role = currentPlayer.chooseRole();
        std::vector<Player *> clockwisePlayers = getTurnOrderPlayers();
        auto it = clockwisePlayers.begin();
        (*it)->leadRole(role);
        for (; it != clockwisePlayers.end(); ++it) {
            (*it)->followOrDissentRole(role);
        }
    }
    while (!(gameState.endCondition()));
}

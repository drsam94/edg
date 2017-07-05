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
        for (auto it = clockwisePlayers.begin(); it != clockwisePlayers.end(); ++it) {
            (*it)->doRole(role, it == clockwisePlayers.begin());
        }
        currentPlayer.cleanupPhase();
        ++gameState.currentPlayerIndex;
    }
    while (!(gameState.endCondition()));
}

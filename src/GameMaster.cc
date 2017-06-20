// (c) 2017 Sam Donow
#include "GameMaster.h"

GameMaster::gameLoop()
{
    gameState.init(4);
    do {
        Player &currentPlayer = gameState.nextPlayer();
        currentPlayer.startTurn();
        while (currentPlayer.hasActionsToPlay()) {
            std::optional<Action> action = currentPlayer.getActionChoice();
            if (!action) break;

            player.playAction(*action);
        }
        Role role = player.chooseRole();
        std::vector<Player *> clockwisePlayers = player.getTurnOrderPlayers();
        auto it = clockwisePlayers.begin();
        (*it)->leadRole(role);
        for (; it != clockwisePlayers.end(); ++it) {
            (*it)->followOrDissentRole(role);
        }
    }
    while (!(gameState.endCondition() && currentPlayer.wasFirstPlayer()))
}

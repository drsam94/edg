// (c) 2017 Sam Donow
#include "GameMaster.h"
#include <iostream>

void GameMaster::init(size_t numPlayers) {
    gameState.init(2);
    for (PlayerState &state : gameState.players) {
        players.emplace_back(&gameState, &state);
    }
}

std::vector<Player *> GameMaster::getTurnOrderPlayers() {
    std::vector<Player *> ret;
    const size_t N = gameState.numPlayers();
    for (size_t i = 0; i < N; ++i) {
        ret.push_back(&players[(i + gameState.currentPlayerIndex) % N]);
    }
    return ret;
}

void GameMaster::gameLoop()
{
    std::cout << "Beginning 2 Player game...\n";
    init(2);
    do {
        std::cout << "Beginning of Player " << static_cast<int>(gameState.currentPlayerIndex) << "'s turn\n";
        Player &currentPlayer = getCurrentPlayer();
        currentPlayer.startTurn();
        while (currentPlayer.hasActionsToPlay()) {
            ActionID action = currentPlayer.getActionChoice();
            if (!action.valid()) break;

            currentPlayer.playAction(action);
        }
        Role role = currentPlayer.chooseRole();
        const std::vector<Player *> clockwisePlayers = getTurnOrderPlayers();
        for (auto it = clockwisePlayers.begin(); it != clockwisePlayers.end(); ++it) {
            (*it)->doRole(role, it == clockwisePlayers.begin());
        }
        currentPlayer.cleanupPhase();
        ++gameState.currentPlayerIndex;
        gameState.currentPlayerIndex %= clockwisePlayers.size();
    }
    while (!(gameState.endCondition()));
}

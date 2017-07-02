// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "Player.h"
#include "State.h"
#include <unordered_map>
#include <vector>

class ChoiceAdapter;
class Action;

class GameMaster {
  private:
    GameState gameState;
    Phase currentPhase;
    std::vector<Player> players;
    Player &getCurrentPlayer() {
        return players[gameState.currentPlayerIndex];
    }
  public:
    void gameLoop();
    void init(size_t numPlayers) {}
    std::vector<Player *> getTurnOrderPlayers() { return {}; }
 };

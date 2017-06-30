// (c) 2017 Sam Donow
#include "CoreEnums.h"
#include "Player.h"
#include "State.h"
#include <unordered_map>
#include <vector>

class ChoiceAdapter;
class Action;

class GodBook {
  private:
      std::unordered_map<ActionID, std::unique_ptr<Action>> actionMap;
};

class GameMaster {
    GameState gameState;
    uint8_t currentPlayerIndex;
    Phase currentPhase;
    std::vector<Player> adapters;
  public:
    void gameLoop();
};

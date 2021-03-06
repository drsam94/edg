// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "ChoiceAdapter.h"
#include <memory>

class GameState;
class PlayerState;
class Player {
  private:
    Phase currentPhase;
    uint8_t actionsLeft;
    std::unique_ptr<ChoiceAdapter> adapter;
  public:
    GameState &gameState;
    PlayerState &state;

    Player(GameState &_gameState, PlayerState &_state) :
      adapter(new TTYChoiceAdapter(_gameState, _state)), gameState(_gameState), state(_state)  {}
    /// Interface exposed to GameMaster
    ActionID getActionChoice();
    Role chooseRole();
    void doRole(Role role, bool isLeader);
    void startTurn() { currentPhase = Phase::Action; actionsLeft = 1; }
    bool hasActionsToPlay() { return actionsLeft > 0; }
    bool playAction(ActionID action);
    void leadRole(Role role);
    void followOrDissentRole(Role role);

    /// Interface exposed to action effects
    /// TODO: separate this to be separate from above interface
    /// but don't focus on that too much right now.
    /// Also, for right now I am going to assume that these methods are only
    /// called if they are illegal. As I expect the ChoiceAdapters to ensure this
    /// generally, I think that's fine, eventually I will probably move to using
    /// exceptions in cases of bad input
    ChoiceAdapter &getAdapter() { return *adapter.get(); }
    void getFighters(int count);
    void settlePlanet(int planetIdx);
    void attackPlanet(int planetIdx);
    void addColony(int planetIdx, ActionID action);
    void produce(int planetIdx, int resourceSlotIdx);
    void trade(int planetIdx, int resourceSlotIdx);
    std::vector<ActionID> removeFromHand(std::vector<int> handIdxs);
    ActionID gainRoleTo(Role role, bool toHand);
    void drawCards(int count);
    void cleanupPhase();
};



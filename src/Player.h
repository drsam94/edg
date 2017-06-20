// (c) 2017 Sam Donow
#include "CoreEnums.h"
#include <memory>
#include <optional>

class Player {
    PlayerState *state;
    ChoiceAdapter adapter;
    Phase currentPhase;
    uint8_t actionsLeft;
  public:
    Player(PlayerState *_state, ChoiceAdapter type) : state(_state),
        adapter(state, type) {}
    std::optional<Action> getActionChoice();
    void startTurn() { currentPhase = Phase::Action; }
    bool hasActionsToPlay() { return actionsLeft > 0; }
    void playAction(const Action &action);
};



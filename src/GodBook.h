// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "Cards.h"
#include <memory>
#include <unordered_map>

class GodBook {
  private:
      static std::unordered_map<ActionID, std::unique_ptr<Action>> actionMap;

  public:
      static const Action &getAction(ActionID id) {
          return *actionMap[id].get();
      }
};



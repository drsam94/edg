// (c) 2017 Sam Donow
#pragma once
#include "Util.h"
#include "CoreEnums.h"
#include <memory>
#include <unordered_map>

class Action;
class Planet;
class GodBook {
  private:
    std::unordered_map<ActionID, std::unique_ptr<Action>> actionMap;
    std::unordered_map<PlanetID, std::unique_ptr<Planet>> planetMap;
    static std::unique_ptr<GodBook> singleton;
  public:
    GodBook();
    static GodBook &instance() {
        if (unlikely(singleton == nullptr)) {
            singleton.reset(new GodBook);
        }
        return *singleton;
    }
    const Action &getAction(ActionID id) {
        return *actionMap[id].get();
    }

    const Planet &getPlanet(PlanetID id) {
        return *planetMap[id].get();
    }
};



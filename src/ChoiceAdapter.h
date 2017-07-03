// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "State.h"
#include <vector>
/// Currently, for all choices, we use an ad-hoc vector-of-ints format,
/// which tends to work well as most choices tend to collection of indices, etc
/// I also want the data to be very simple, as these choices will be returned by
/// any input mechanism. The three mechanisms I intend to support are:
/// 1) A command line interface
/// 2) A web interface
/// 3) (potentially) an AI/ML interface
class ChoiceAdapter {
  public:
    virtual int chooseAction() { return {}; }

    virtual std::vector<int> getDissentBoostFollowChoice(Role role, bool lead) { return {}; }
    virtual size_t chooseOneOfPlanetCards(const std::vector<PlanetID> &planets) { return {}; }
    virtual size_t chooseOneOfFDPlanets(const std::vector<PlanetState> &planets) { return {}; }
    virtual std::vector<int> placeColonies(const std::vector<ActionID> &colonies, const std::vector<PlanetState> &planets) { return {}; }
    virtual std::vector<int> chooseResourceSlots(size_t symcount, const std::vector<PlanetState> &planets, bool emptySlots) { return {}; };
    virtual ActionID getResearchChoice(size_t symcount, const TechTree &techs, const std::vector<PlanetState> &planets) { return {}; };
};

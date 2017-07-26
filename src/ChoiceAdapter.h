// (c) 2017 Sam Donow
#pragma once
#include "CoreEnums.h"
#include "State.h"
#include <vector>
#include <tuple>
/// Currently, for all choices, we use an ad-hoc vector-of-ints format,
/// which tends to work well as most choices tend to collection of indices, etc
/// I also want the data to be very simple, as these choices will be returned by
/// any input mechanism. The three mechanisms I intend to support are:
/// 1) A command line interface
/// 2) A web interface
/// 3) (potentially) an AI/ML interface
class ChoiceAdapter {
  protected:
    const GameState &gState;
    const PlayerState &pState;
  private:
    int generalAlternative(const std::string &choice1, const std::string &choice2) { return {}; }

    // Subclasses must implement these two methods for how to communicate with the playing agent
    virtual int getChoice() = 0;
    virtual std::ostream &display() = 0;
    void displayHand(const std::vector<ActionID> &hand);
  public:
    ChoiceAdapter(const GameState &gs, const PlayerState &ps) : gState(gs), pState(ps) {}
    ActionID chooseAction(const std::vector<ActionID> &hand);
    std::vector<int> chooseOneOfPlanetCards(const std::vector<PlanetID> &planets);
    std::vector<int> chooseOneOfFDPlanets(const std::vector<PlanetState> &planets);
    std::vector<int> placeColonies(const std::vector<ActionID> &colonies, const std::vector<PlanetState> &planets) { return {}; }
    std::vector<int> chooseResourceSlots(size_t symcount, const std::vector<PlanetState> &planets, bool emptySlots) { return {}; };
    ActionID getResearchChoice(size_t symcount, const TechTree &techs, const std::vector<PlanetState> &planets) { return {}; };
    std::vector<int> nullChoice() { return {}; }
    std::vector<int> chooseRole(const RoleState &roles, int atMost);
    std::vector<int> chooseCardsFromHand(const std::vector<ActionID> &hand, int atMost);
    // Something like this is a justification for keeping the return types fully consistent,
    // allowing for easy composition of decisions
    template<typename FnT1, typename FnT2, typename TupleT1, typename TupleT2>
    std::vector<int> composedAlternatives(const std::string &&alt1Desc,
            FnT1 &&fn1, TupleT1 &&tuple1, const std::string &&alt2Desc,
            FnT2 &&fn2, TupleT2 &&tuple2) {
        std::vector<int> ret;
        ret.push_back(generalAlternative(alt1Desc, alt2Desc));
        std::vector<int> additionalChoices;
        if (ret[0]) {
            additionalChoices = std::apply(fn1, std::tuple_cat(
                        std::make_tuple(this), tuple1));
        } else {
            additionalChoices = std::apply(fn2, std::tuple_cat(
                        std::make_tuple(this), tuple2));
        }
        ret.insert(ret.end(), additionalChoices.begin(), additionalChoices.end());
        return ret;
    }
    std::vector<int> getDissentBoostFollowChoice(Role role, bool lead,
            const std::vector<ActionID> &hand);

};

class TTYChoiceAdapter : public ChoiceAdapter {
  private:
    std::ostream &out;
    std::istream &in;

    std::ostream &display() override;
    int getChoice() override;
  public:
    TTYChoiceAdapter(const GameState &gs, const PlayerState &ps,
      std::ostream &_out = std::cout, std::istream &_in = std::cin) : ChoiceAdapter(gs, ps), out(_out), in(_in) {}

};

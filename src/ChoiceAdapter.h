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
  private:
    virtual int generalAlternative(const std::string &choice1, const std::string &choice2) { return {}; }
  public:
    virtual ActionID chooseAction(const std::vector<ActionID> &hand) { return {}; }
    // could/should be pair<bool, vector<ActionID>>
    virtual std::vector<int> getDissentBoostFollowChoice(Role role, bool lead, const std::vector<ActionID> &hand) { return {}; }
    virtual std::vector<int> chooseOneOfPlanetCards(const std::vector<PlanetID> &planets) { return {}; }
    virtual std::vector<int> chooseOneOfFDPlanets(const std::vector<PlanetState> &planets) { return {}; }
    virtual std::vector<int> placeColonies(const std::vector<ActionID> &colonies, const std::vector<PlanetState> &planets) { return {}; }
    virtual std::vector<int> chooseResourceSlots(size_t symcount, const std::vector<PlanetState> &planets, bool emptySlots) { return {}; };
    virtual ActionID getResearchChoice(size_t symcount, const TechTree &techs, const std::vector<PlanetState> &planets) { return {}; };
    std::vector<int> nullChoice() { return {}; }

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
};

class TTYChoiceAdapter : public ChoiceAdapter {
  private:
    std::ostream &out;
    std::istream &in;

    void displayHand(const std::vector<ActionID> &hand);
  public:
    TTYChoiceAdapter(std::ostream &_out = std::cout, std::istream &_in = std::cin) : out(_out), in(_in) {}

    ActionID chooseAction(const std::vector<ActionID> &hand) override;
    std::vector<int> getDissentBoostFollowChoice(Role role, bool lead, const std::vector<ActionID> &hand) override;
};

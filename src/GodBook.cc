#include "GodBook.h"
#include "CardDetails.h"

std::unique_ptr<GodBook> GodBook::singleton;

GodBook::GodBook() {
    #define MAP_ACTION(ActionType) \
    actionMap[ActionID::ActionType].reset(new ActionType());

    #define MAP_PLANET(PlanetType) \
    planetMap[PlanetID::PlanetType].reset(new PlanetType());
    ACT_ON_ALL_ACTIONS(MAP_ACTION)
    ACT_ON_ALL_PLANETS(MAP_PLANET)
}


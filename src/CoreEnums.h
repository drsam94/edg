// (c) Sam Donow 2017
#pragma once
#include "Enum.h"

ENUM(Symbol, char,
        Survey,
        Warfare,
        Colonize,
        Produce,
        Trade,
        Research,
        HandSizePlus
        //EXPANSION: ,
        // Resource,
        // Fighter,
        // Destroyer,
        // Battlecruiser
    )

ENUM(PlanetType, char, Fertile, Metallic, Advanced)

// Same as Symbol until expansion
ENUM(Role, char, Survey, Warfare, Colonize, Produce, Trade, Research)

ENUM(TechType, char, Action, Permanent)

ENUM(Resource, char, Silicon, Rock, Water, Grain)

ENUM(Phase, char, Action, RoleLeader, RoleFollower, Cleanup, None)


#define ACT_ON_ALL_PLANETS(m) \
    m(StartSaturn1) \
    m(StartSaturn2) \
    m(StartEarth1) \
    m(StartEarth2) \
    m(StartMars1) \
    m(StartMars2)

#define ACT_ON_ALL_ACTIONS(m) \
    m(Survey) \
    m(Warfare) \
    m(Colonize) \
    m(ProduceTrade) \
    m(Research) \
    m(Politics)

#define COMMA(x) x,

ENUM(ActionID, unsigned, ACT_ON_ALL_ACTIONS(COMMA) Invalid)

ENUM(PlanetID, unsigned, ACT_ON_ALL_PLANETS(COMMA) Invalid)

inline ActionID RoleToAction(Role role) {
    switch (role) {
        case Role::Survey:
            return ActionID::Survey;
        case Role::Warfare:
            return ActionID::Warfare;
        case Role::Colonize:
            return ActionID::Colonize;
        case Role::Produce:
        case Role::Trade:
            return ActionID::ProduceTrade;
        case Role::Research:
            return ActionID::Research;
        default:
            return ActionID::Unset;
    }
}

inline Symbol RoleToSymbol(Role role) {
    switch (role) {
        case Role::Survey:
            return Symbol::Survey;
        case Role::Warfare:
            return Symbol::Warfare;
        case Role::Colonize:
            return Symbol::Colonize;
        case Role::Produce:
            return Symbol::Produce;
        case Role::Trade:
            return Symbol::Trade;
        case Role::Research:
            return Symbol::Research;
        default:
            return Symbol::Unset;
    }
}

#undef COMMA

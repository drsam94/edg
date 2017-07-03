// (c) Sam Donow 2017
#pragma once
#include "Enum.h"

ENUM(Symbol, char,
        Survey,
        Warfare,
        Colonize,
        Produce,
        Trade,
        Research
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

ENUM(ActionID, unsigned,
    Survey,
    Warfare,
    Colonize,
    ProduceTrade,
    Research,
    Politics)

#define ACT_ON_ALL_ACTIONS(m) \
    m(Survey) \
    m(Warfare) \
    m(Colonize) \
    m(ProduceTrade) \
    m(Research) \
    m(Politics)

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

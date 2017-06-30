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
ENUM(Role, char, Survery, Warfare, Colonize, Produce, Trade, Research)

ENUM(TechType, char, Action, Permanent)

ENUM(Resource, char, Silicon, Rock, Water, Grain)

ENUM(Phase, char, Action, RoleLeader, RoleFollower, Cleanup, None)

ENUM(ActionID, unsigned,
    Survey,
    Warfare,
    Colonize,
    ProduceTrade,
    Research)

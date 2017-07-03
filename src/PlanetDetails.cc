// (c) Sam Donow
#include "CardDetails.h"
#define DETAILS(Type, title, planetType, colonies, fighters, inf, resources, symArr) \
Type::Type() : Planet(CardSpec{title, "", symArr, inf, PlanetID::Type + Planet::PlanetIDOffset}, \
            PlanetType::planetType, colonies, fighters, resources) {} 
#define RES_3(t1, t2, t3) std::array<Resource, 3>{{Resource::t1, Resource::t2, Resource::t3}}
#define RES_2(t1, t2) RES_3(t1, t2, Unset)
#define RES_1(t1) RES_2(t1, Unset)
#define RES_0() RES_1(Unset)

DETAILS(StartSaturn1, "StartSaturn1", Advanced, 2, 2, 2, RES_1(Silicon), SYM_0())
DETAILS(StartSaturn2, "StartSaturn2", Advanced, 2, 2, 2, RES_1(Silicon), SYM_0())
DETAILS(StartEarth1, "StartEarth1", Fertile, 2, 2, 2, RES_1(Water), SYM_0())
DETAILS(StartEarth2, "StartEarth2", Fertile, 2, 2, 2, RES_1(Grain), SYM_0())
DETAILS(StartMars1, "StartMars1", Metallic, 2, 2, 2, RES_1(Rock), SYM_0())
DETAILS(StartMars2, "StartMars2", Metallic, 2, 2, 2, RES_1(Rock), SYM_0())

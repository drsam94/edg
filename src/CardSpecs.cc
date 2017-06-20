#include "CardDetails.h"

#define ACTION(Type, title, symArr, inf, perm, rules) \
    CardSpec Type::spec{title,rules,symArr,inf}; \
    bool Type::isPermanent = perm; \
    ActionID Type::ID = ActionID::Type;

#define SYM(type) {Symbol::type, Symbol::Unset, Symbol::Unset}

ACTION(Survey, "Survey", SYM(Survey), 0, false, "...")
ACTION(Warfare, "Warfare", SYM(Warfare), 0, false, "...")

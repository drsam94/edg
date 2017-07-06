#include "Cards.h"

inline char symChar(Symbol sym) {
    switch (sym) {
        case Symbol::Survey : return 'S';
        case Symbol::Warfare: return 'W';
        case Symbol::Colonize: return 'C';
        case Symbol::Produce: return 'P';
        case Symbol::Trade: return 'T';
        case Symbol::Research: return 'R';
        case Symbol::HandSizePlus: return 'H';
        default: return '_';
    }
}
std::ostream &operator<<(std::ostream &os, const CardSpec &spec) {
    os << spec.title << ": ";
    for (const Symbol &sym : spec.symbols) {
        if (!sym.valid()) break;
        os << symChar(sym) << ' ';
    }
    if (spec.influence > 0) {
        os << " (" << spec.influence << ')';
    }
    return os;
}

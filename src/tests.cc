// (c) 2017 Sam Donow
// Beginning of a bare-bones test suite
#include "CoreEnums.h"
#include "util/Util.h"
#include <stdio.h>
int main() {
    int ret = 0;
#define TS_EQUALS_S(x, y) \
    if (x != y) {\
        ret = 1; \
        fprintf(stderr, "expected #x == #y, instead found \"%s\" != \"%s\"", +x, +y); \
    }
    // TODO: add a lot more tests; this currently just does the most rudimentary sanity check of Enums possible
    Role role = Role::Survey;
    TS_EQUALS_S(role.str(), std::string("Survey"));
    return ret;
}

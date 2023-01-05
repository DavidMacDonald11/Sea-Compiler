#ifndef FAULT_H
#define FAULT_H

#include "../util.h"
#include "../util/component.h"

namespace Fault {
    extern vector<str> warnings;
    extern vector<str> errors;
    extern str failure;

    class CompilerFailure : public std::exception {};

    str toString();
    void warn(Component& c, const str& message);
    void error(Component& c, const str& message);
    CompilerFailure fail(Component& c, const str& message);
    void check();
    void reset();
}

#endif

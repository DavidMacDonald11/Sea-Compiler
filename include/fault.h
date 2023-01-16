#ifndef FAULT_H
#define FAULT_H

#include "util.h"
#include "parser/component.h"

struct Fault {
    class CompilerFailure : public std::exception {};

    vector<str> warnings;
    vector<str> errors;
    str failure;

    str toString();
    void warn(Component& c, const str& message);
    void error(Component& c, const str& message);
    CompilerFailure fail(Component& c, const str& message);
    void check();
    void reset();
};

#endif

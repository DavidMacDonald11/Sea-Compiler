#ifndef FAULT_H
#define FAULT_H

#include "util.h"
#include "parser/component.h"

struct Fault {
    class CompilerFailure : public std::exception {};

    nat stage;
    vector<str> warnings;
    vector<str> errors;
    str failure;

    Fault();

    str toString();
    void warn(Component& c, const str& message);
    void error(Component& c, const str& message);
    CompilerFailure fail(Component& c, const str& message);
    void check();

private:
    str act(Component& c, const str& message, const str& label);
};

#endif

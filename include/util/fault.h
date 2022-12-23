#ifndef FAULT_H
#define FAULT_H

#include "../util.h"
#include "../lexer/token.h"

namespace Fault {
    extern vector<str> warnings;
    extern vector<str> errors;
    extern str failure;

    class CompilerFailure : public std::exception {};

    str toString();
    void warn(Token& token, const str& message);
    void error(Token& token, const str& message);
    CompilerFailure fail(Token& token, const str& message);
    void check();
    void reset();
}

#endif

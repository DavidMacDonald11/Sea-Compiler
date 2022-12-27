#ifndef TOKEN_H
#define TOKEN_H

#include "source_line.h"
#include "../util.h"

class Token {
public:
    enum Type {PUNC, OP, NUM, CHAR, STR, IDENTIFIER, KEYWORD, NONE};

    SourceLine& line;
    Type type;
    str string;
    Locale locale;

    Token(SourceLine& line, Type type);

    str toString() const;
    bool of(list<Type>) const;
    bool has(list<str>) const;
    void mark();

    static const str PUNC_SYMS;
    static const vector<str> PREFIX_UNARY_OPS;
    static const vector<str> BINARY_OPS;
    static const vector<str> COMPARE_OPS;
    static const vector<str> OPERATORS;
    static const str OP_SYMS;
    static const str S_NUM_SYMS;
    static const str NUM_SYMS;
};

#endif

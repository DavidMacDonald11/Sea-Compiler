#ifndef TOKEN_H
#define TOKEN_H

#include "source_line.h"
#include "../util.h"
#include "../util/component.h"

class Token : public Component {
public:
    enum Type {PUNC, OP, NUM, CHAR, STR, IDENTIFIER, KEYWORD, NONE};

    SourceLine& line;
    Type type;
    str string;
    Locale locale;

    Token(SourceLine& line, Type type);

    bool of(list<Type> types) const;
    bool has(list<str> values) const;
    str toString() const override;
    str tree(str) const override;
    vector<SourceLine*> lines() const override;
    str raw() const override;
    void mark() override;

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

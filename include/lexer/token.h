#ifndef TOKEN_H
#define TOKEN_H

#include "source-line.h"
#include "../util.h"
#include "../util/component.h"

struct Token : public Component {
    enum Type {PUNC, NUM, OP, CHAR, STR, IDENTIFIER, KEYWORD, NONE};

    SourceLine& line;
    Type type;
    str string;
    Locale locale;

    Token(SourceLine& line, Type type);

    bool of(vector<Type> types) const;
    bool has(vector<str> values) const;
    str toString() const override;
    str tree(str) const override;
    vector<SourceLine*> lines() const override;
    str raw() const override;
    void mark() override;

    static const str PUNC_SYMS;
    static const str S_NUM_SYMS;
    static const str NUM_SYMS;
    static const vector<str> POSTFIX_OPS;
    static const vector<str> PREFIX_UNARY_OPS;
    static const vector<str> BINARY_OPS;
    static const vector<str> COMPARE_OPS;
    static const vector<str> OPERATORS;
    static const str OP_SYMS;
    static const str S_IDENTIFIER_SYMS;
    static const str IDENTIFIER_SYMS;
    static const vector<str> PRIMARY_KEYWORDS;
    static const vector<str> TYPE_KEYWORDS;
    static const vector<str> KEYWORDS;

    static const vector<str> LINE_ENDS;
};

#endif

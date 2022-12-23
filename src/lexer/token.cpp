#include <map>
#include "lexer/token.h"

std::map<Token::Type, char> typeLabels {
    {Token::PUNC, 'P'},
    {Token::OP, 'O'},
    {Token::NUM, 'N'},
    {Token::CHAR, 'C'},
    {Token::STR, 'S'},
    {Token::IDENTIFIER, 'I'},
    {Token::KEYWORD, 'K'},
    {Token::NONE, '\0'},
};


Token::Token(SourceLine& line, Type type)
: line(line), type(type), locale(line.newLocale()) {
    string = line.string.substr(locale[0], locale[1] - locale[0]);
}

str Token::toString() const {
    str string = (self.string == "") ? "EOF" : self.string;
    return fmt::format("{}'{}'", typeLabels[type], string);
}

bool Token::of(list<Type> args) const {
    return in(type, args);
}

bool Token::has(list<str> args) const {
    return in(string, args);
}

void Token::mark() {
    line.mark(self);
}


const str Token::PUNC_SYMS = "{}()";

const vector<str> Token::PREFIX_UNARY_OPS = {
    "+", "-"
};

const vector<str> Token::BINARY_OPS = {
    "*", "/", "+", "-"
};

const vector<str> Token::COMPARE_OPS = {
    "<", ">", "<=", ">=", "==", "!="
};

const vector<str> Token::OPERATORS(mergeAll<str>({
    Token::PREFIX_UNARY_OPS,
    Token::BINARY_OPS,
    Token::COMPARE_OPS
}));

const str Token::OP_SYMS(fmt::format("{}", fmt::join(Token::OPERATORS, "")));

#include <map>
#include "lexer/source_line.h"
#include "lexer/token.h"
#include "util.h"

std::map<Token::Type, char> typeLabels {
    {Token::PUNC, 'P'},
    {Token::OP, 'O'},
    {Token::NUM, 'N'},
    {Token::CHAR, 'C'},
    {Token::STR, 'S'},
    {Token::IDENTIFIER, 'I'},
    {Token::KEYWORD, 'K'},
    {Token::NONE, '?'},
};


Token::Token(SourceLine& line, Type type)
: line(line), type(type), locale(line.newLocale()) {
    string = line.string.substr(locale[0], locale[1] - locale[0]);
}

bool Token::of(list<Type> types) const {
    return in(type, types);
}

bool Token::has(list<str> values) const {
    return in(string, values);
}

str Token::toString() const {
    str string = (self.string == "") ? "EOF" : self.string;
    string = replaceStr(string, "\n", "\\n");

    return fmt::format("{}'{}'", typeLabels[type], string);
}

str Token::tree(str) const { return toString(); }

vector<SourceLine*> Token::lines() const { return {&line}; }

str Token::raw() const { return line.raw(); }

void Token::mark() { line.mark(self); }


const str Token::PUNC_SYMS("{}(),;\n");
const str Token::S_NUM_SYMS("0123456789.");
const str Token::NUM_SYMS(fmt::format("{}", S_NUM_SYMS));

const vector<str> Token::POSTFIX_OPS {
    ".", "?.", "::"  
};

const vector<str> Token::PREFIX_UNARY_OPS {
    "+", "-", "!", "^", "@"
};

const vector<str> Token::BINARY_OPS {
    "*", "/", "%", "+", "-",
    "<<", ">>", "&", "$", "|"
};

const vector<str> Token::COMPARE_OPS {
    "<", ">", "<=", ">=", "==", "!="
};

const vector<str> Token::OPERATORS(mergeAll<str>({
    POSTFIX_OPS,
    PREFIX_UNARY_OPS,
    COMPARE_OPS,
    BINARY_OPS,
}));

const str Token::OP_SYMS(fmt::format("{}", fmt::join(OPERATORS, "")));
const str Token::S_IDENTIFIER_SYMS("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const str Token::IDENTIFIER_SYMS(fmt::format("{}{}", S_IDENTIFIER_SYMS, "0123456789"));

const vector<str> Token::PRIMARY_KEYWORDS {
    "true", "false", "null"
};

const vector<str> Token::TYPE_KEYWORDS {
    "wild", "bool", "byte", "char",
    "short", "int", "long", "nat", "float"
};

const vector<str> Token::KEYWORDS(mergeAll<str>( {
    PRIMARY_KEYWORDS,
    TYPE_KEYWORDS,
    {
        "not", "and", "xor", "or", "as", "if", "else"
    }
}));

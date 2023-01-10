#include <map>
#include "lexer/source-line.h"
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

bool Token::of(vector<Type> types) const {
    return in(type, types);
}

bool Token::has(vector<str> values) const {
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


const str Token::PUNC_SYMS("{}[](),;`");
const str Token::S_NUM_SYMS("0123456789.");
const str Token::NUM_SYMS(fmt::format("{}xXabcdefABCDEF", S_NUM_SYMS));

const vector<str> Token::POSTFIX_OPS {
    ".", "?." 
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

const vector<str> Token::ASSIGN_OPS {
    "*=", "/=", "%=", "+=", "-=",
    "<<=", ">>=", "&=", "$=", "|=", "="
};

const vector<str> Token::PUNC_OPS {
    "::", "->", "?", ".", "..."
};

const vector<str> Token::OPERATORS(mergeAll<str>({
    POSTFIX_OPS,
    PREFIX_UNARY_OPS,
    BINARY_OPS,
    COMPARE_OPS,
    ASSIGN_OPS,
    PUNC_OPS
}));

const str Token::OP_SYMS(fmt::format("{}", fmt::join(OPERATORS, "")));
const str Token::S_IDENTIFIER_SYMS("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
const str Token::IDENTIFIER_SYMS(fmt::format("{}{}", S_IDENTIFIER_SYMS, "0123456789"));


const vector<str> Token::PRIMARY_KEYWORDS {
    "true", "false", "null"
};

const vector<str> Token::UNARY_KEYWORDS {
    "await", "alignof", "sizeof"
};

const vector<str> Token::ALLOC_KEYWORDS {
    "alloc", "realloc", "unalloc"
};

const vector<str> Token::TYPE_SPECIFIER_KEYWORDS {
    "wild", "bool", "byte", "char",
    "int16", "int", "int32", "int64", 
    "nat16", "nat", "nat32", "nat64",
    "real32", "real", "real64",
    "cplex32", "cplex", "cplex64"
};

const vector<str> Token::TYPE_QUALIFIER_KEYWORDS {
    "const", "unique", "volatile", "atomic"  
};

const vector<str> Token::TYPE_NAME_KEYWORDS(mergeAll<str>({
    TYPE_SPECIFIER_KEYWORDS,
    TYPE_QUALIFIER_KEYWORDS,
    {"alignas", "async", "fun"}
}));

const vector<str> Token::VISIBILITY_KEYWORDS {
    "export", "local"  
};

const vector<str> Token::STORAGE_KEYWORDS {
    "register", "static", "future"  
};

const vector<str> Token::KEYWORDS(mergeAll<str>( {
    PRIMARY_KEYWORDS,
    UNARY_KEYWORDS,
    ALLOC_KEYWORDS,
    TYPE_NAME_KEYWORDS,
    {
        "not", "and", "xor", "or", "as", "if", "else",
        "inline", "deviant", "assert",
        "enum", "struct", "union",
        "pass", "continue", "break", "return",
        "while", "do", "for", "then",
        "compare", "using", "with",
        "alias", "import", "include", "from",
        "to", "by", "define"
    }
}));

const vector<str> Token::LINE_ENDS {    
    ";", "\n", ""  
};

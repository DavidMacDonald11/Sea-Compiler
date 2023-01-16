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
    {Token::C_TOKENS, '#'},
    {Token::NONE, '?'},
};

std::map<Token::Type, str> typeNames {
    {Token::PUNC, "Punctuator"},
    {Token::OP, "Operator"},
    {Token::NUM, "Number"},
    {Token::CHAR, "Character"},
    {Token::STR, "String"},
    {Token::IDENTIFIER, "Identifier"},
    {Token::KEYWORD, "Keyword"},
    {Token::C_TOKENS, "C Tokens"}
};


Token::Token(SourceLine& line, Type type)
: line(line), type(type), locale(line.newLocale()) {
    string = line.string.substr(locale[0], locale[1] - locale[0]);
}

Token::Token(SourceLine& line, Type type, str string, Locale locale)
: line(line), type(type), string(string), locale(locale) {}

Token::Token(const Token& token)
: Token(token, token.line) {}

Token::Token(const Token& token, SourceLine& line) 
: line(line), type(token.type), string(token.string), locale(token.locale) {}

Token& Token::operator=(const Token& token) {
    if(&line != &token.line) throw CopyError();

    type = token.type;
    string = token.string;
    locale = token.locale;
    
    return self;
}

bool Token::of(vector<Type> types) const {
    return in(type, types);
}

bool Token::has(vector<str> values) const {
    vector<str> vals;
    for(str value : values) vals.push_back((value == "EOF")? "" : value);

    return in(string, vals);
}

bool Token::isInt() const {
    return of({NUM}) and not in('.', string);
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
str Token::typeToString(Type type) { return typeNames[type]; }


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
    "::", "->", "?", ".", "...", "#", "###"
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
    "const", "unique", "volatile", "atomic", "future" 
};

const vector<str> Token::FUNCTION_KEYWORDS {
    "inline", "deviant", "async"  
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
    "register", "static", "threaded"
};

const vector<str> Token::DECLARATION_KEYWORDS(mergeAll<str>({
    TYPE_NAME_KEYWORDS,
    VISIBILITY_KEYWORDS,
    STORAGE_KEYWORDS
}));

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
        "to", "by"
    }
}));

const vector<str> Token::LINE_ENDS {    
    ";", "\n", "EOF"  
};

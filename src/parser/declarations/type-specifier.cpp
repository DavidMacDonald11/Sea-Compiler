#include <map>
#include "parser/declarations/type-specifier.h"
#include "parser/node.h"

std::map<str, str> typeMap {
    {"wild", "void"},
    {"bool", "_Bool"},
    {"byte", "unsigned char"}, 
    {"char", "signed char"},
    {"int16", "short"},
    {"int", "int"},
    {"int32", "long"},
    {"int64", "long long"},
    {"nat16", "unsigned short int"},
    {"nat", "unsigned int"},
    {"nat32", "unsigned long int"},
    {"nat64", "unsigned long long int"},
    {"real32", "float"},
    {"real", "double"},
    {"real64", "long double"},
    {"cplex32", "_Complex float"},
    {"cplex", "_Complex double"},
    {"cplex64", "_Complex long double"}
};


TypeSpecifier::TypeSpecifier(Token& token) 
: PrimaryNode(token) {}

Node* TypeSpecifier::construct() {
    Token& token = parser->expectingHas(Token::TYPE_SPECIFIER_KEYWORDS);
    return new TypeSpecifier(token);
}

Transpiler::Line TypeSpecifier::transpile() {
    return {token.string, typeMap[token.string]};
}

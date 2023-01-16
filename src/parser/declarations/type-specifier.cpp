#include <map>
#include "parser/declarations/atomic-type-specifier.h"
#include "parser/declarations/function-type-specifier.h"
#include "parser/declarations/type-specifier.h"
#include "parser/expressions/primary-expression.h"
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


TypeSpecifier::TypeSpecifier(Token* token, Node* node) 
: token(token), node(node) {}

TypeSpecifier::~TypeSpecifier() {
    delete node;
}

Nodes TypeSpecifier::nodes() const {
    return token? Nodes{token} : Nodes{node};
}

Node* TypeSpecifier::construct(Parser& parser) {
    if(parser.next().has(Token::TYPE_SPECIFIER_KEYWORDS)) 
        return new TypeSpecifier(&parser.take());

    if(parser.next().has({"atomic"})) return AtomicTypeSpecifier::construct(parser);
    if(parser.next().has({"async", "fun"})) return FunctionTypeSpecifier::construct(parser);

    return new TypeSpecifier(nullptr, FileIdentifier::construct(parser));
}

Transpiler::Line TypeSpecifier::transpile(Transpiler& transpiler) {
    if(token) return {token->string, typeMap[token->string]};
    return node->transpile(transpiler);
}

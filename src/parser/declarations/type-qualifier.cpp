#include <map>
#include "parser/declarations/type-qualifier.h"

std::map<str, str> qualifierMap {
    {"const", "const"},
    {"unique", "restrict"},
    {"volatile", "volatile"},
    {"atomic", "_Atomic"}
};

TypeQualifier::TypeQualifier(Token& token)
: PrimaryNode(token) {}

Node* TypeQualifier::construct(Parser& parser) {
    Token& token = parser.expectingHas(Token::TYPE_QUALIFIER_KEYWORDS);
    return new TypeQualifier(token);
}

Transpiler::Line TypeQualifier::transpile(Transpiler&) {
    return {"", qualifierMap[token.string]};
}

#include "parser/declarations/align-specifier.h"
#include "parser/declarations/type-name.h"
#include "parser/expressions/expression.h"

vector<Component*> AlignSpecifier::nodes() const { return {&alignOf}; }

AlignSpecifier::AlignSpecifier(Node& alignOf)
: alignOf(alignOf) {}

AlignSpecifier::~AlignSpecifier() {
    delete &alignOf;
}

Node* AlignSpecifier::construct() {
    parser->expectingHas({"alignas"});
    parser->expectingHas({"("});

    Node* alignOf = (parser->next().has(Token::TYPE_NAME_KEYWORDS))? 
        TypeName::construct() : Expression::construct();

    parser->expectingHas({")"});
    return new AlignSpecifier(*alignOf);
}

Transpiler::Line AlignSpecifier::transpile() {
    return alignOf.transpile().add("_Alignas(", ")");
}

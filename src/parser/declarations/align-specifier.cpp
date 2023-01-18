#include "parser/declarations/align-specifier.h"
#include "parser/declarations/type-name.h"
#include "parser/expressions/expression.h"

AlignSpecifier::AlignSpecifier(Node& alignOf)
: alignOf(alignOf) {}

AlignSpecifier::~AlignSpecifier() {
    delete &alignOf;
}

Nodes AlignSpecifier::nodes() const { return {&alignOf}; }

Node* AlignSpecifier::construct(Parser& parser) {
    parser.expectingHas({"alignas"});
    parser.expectingHas({"<"}).type = Token::PUNC;

    Node* alignOf = (parser.next().has(Token::TYPE_NAME_KEYWORDS))? 
        TypeName::construct(parser) : Expression::construct(parser);

    parser.expectingHas({">"}).type = Token::PUNC;
    return new AlignSpecifier(*alignOf);
}

Transpiler::Line AlignSpecifier::transpile(Transpiler& transpiler) {
    return alignOf.transpile(transpiler).add("_Alignas(", ")");
}

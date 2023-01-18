#include "parser/declarations/qualifier-type-specifier.h"
#include "parser/declarations/type-name.h"
#include "parser/declarations/type-qualifier.h"
#include "transpiler/transpiler.h"

QualifierTypeSpecifier::QualifierTypeSpecifier(Node& qualifier, Node& type)
: qualifier(qualifier), type(type) {}

QualifierTypeSpecifier::~QualifierTypeSpecifier() {
    delete &qualifier;
    delete &type;
}

Nodes QualifierTypeSpecifier::nodes() const {
    return {&qualifier, &type};
}

Node* QualifierTypeSpecifier::construct(Parser &parser) {
    Node& qualifier = *TypeQualifier::construct(parser);
    
    parser.expectingHas({"<"}).type = Token::PUNC;
    Node& type = *TypeName::construct(parser);
    parser.expectingHas({">"}).type = Token::PUNC;

    return new QualifierTypeSpecifier(qualifier, type);
}

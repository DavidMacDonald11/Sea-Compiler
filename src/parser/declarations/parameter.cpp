#include "parser/declarations/parameter.h"
#include "parser/declarations/abstract-declarator.h"
#include "parser/declarations/declaration-specifiers.h"

Parameter::Parameter(Node& specifiers, Node* declarator)
: specifiers(specifiers), declarator(declarator) {}

Parameter::~Parameter() {
    delete &specifiers;
    delete declarator;
}

Nodes Parameter::nodes() const {
    if(declarator) return {&specifiers, declarator};
    return {&specifiers};
}

Node* Parameter::construct(Parser& parser) {
    DeclarationSpecifiers& specifiers = *static_cast<DeclarationSpecifiers*>(
        DeclarationSpecifiers::construct(parser));

    if(specifiers.visibility) parser.fault.error(*specifiers.visibility, 
        "Visibility cannot be specified for parameter");
    
    if(specifiers.storage and specifiers.storage->has({"static"})) parser.fault.error(
        *specifiers.storage, 
        "'static' storage cannot be specified for parameter");

    return new Parameter(specifiers, AbstractDeclarator::construct(parser));
}


#include "parser/declarations/parameter.h"
#include "parser/declarations/declaration-specifiers.h"

Parameter::Parameter(Node& specifiers, Token& identifier)
: specifiers(specifiers), identifier(identifier) {}

Parameter::~Parameter() {
    delete &specifiers;
}

Nodes Parameter::nodes() const {
    return {&specifiers, &identifier};
}

Node* Parameter::construct(Parser& parser) {
    DeclarationSpecifiers& specifiers = *static_cast<DeclarationSpecifiers*>(
        DeclarationSpecifiers::construct(parser));

    if(specifiers.visibility) parser.fault.error(*specifiers.visibility, 
        "Visibility cannot be specified for parameter");
    
    if(specifiers.storage and specifiers.storage->has({"static"})) parser.fault.error(
        *specifiers.storage, 
        "'static' storage cannot be specified for parameter");

    Token& identifier = parser.expectingOf({Token::IDENTIFIER});
    return new Parameter(specifiers, identifier);
}


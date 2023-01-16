#include "parser/expressions/type-association.h"
#include "parser/expressions/single-expression.h"
#include "parser/declarations/type-name.h"

TypeAssociation::TypeAssociation(Node& type, Node& expression) 
: type(type), expression(expression) {}

TypeAssociation::~TypeAssociation() {
    delete &type;
    delete &expression;
}

Nodes TypeAssociation::nodes() const {
    return {&type, &expression};
}

Node* TypeAssociation::construct(Parser& parser) {
    Token* paren = (parser.next().has({"("}))? &parser.take() : nullptr;
    Node& type = *TypeName::construct(parser);
    if(paren) parser.expectingHas({")"});

    parser.expectingHas({"?"});
    if(parser.next().has({"then"})) parser.take();

    Node& expression = *SingleExpression::construct(parser);
    return new TypeAssociation(type, expression);
}

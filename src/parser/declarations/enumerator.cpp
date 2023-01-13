#include "parser/declarations/enumerator.h"
#include "parser/expressions/single-expression.h"

Enumerator::Enumerator(Token& identifier, Node* expression)
: identifier(identifier), expression(expression) {}

Enumerator::~Enumerator() {
    delete expression;
}

Nodes Enumerator::nodes() const {
    if(expression) return {&identifier, expression};
    return {&identifier};
}

Node* Enumerator::construct() {
    Token& identifier = parser->expectingOf({Token::IDENTIFIER});
    Node* expression = nullptr;

    if(parser->next().has({"="})) {
        parser->take();
        expression = SingleExpression::construct();
    }

    return new Enumerator(identifier, expression);
}

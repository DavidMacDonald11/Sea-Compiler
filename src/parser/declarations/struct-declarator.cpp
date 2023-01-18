#include "parser/declarations/struct-declarator.h"
#include "parser/expressions/single-expression.h"

StructDeclarator::StructDeclarator(Token* identifier, Node* expression)
: identifier(identifier), expression(expression) {}

StructDeclarator::~StructDeclarator() {
    delete expression;
}

Nodes StructDeclarator::nodes() const {
    if(not identifier) return {expression};
    if(expression) return {identifier, expression};
    return {identifier};
}

Node* StructDeclarator::construct(Parser& parser) {
    if(parser.next().has({"using"})) {
        parser.take();
        return new StructDeclarator(nullptr, SingleExpression::construct(parser));
    }

    Token* identifier = nullptr;
    Node* expression = nullptr;

    if(parser.next().of({Token::IDENTIFIER})) identifier = &parser.take();

    if(parser.next().has({"using"})) {
        parser.take();
        expression = SingleExpression::construct(parser);
    }

    return new StructDeclarator(identifier, expression);
}

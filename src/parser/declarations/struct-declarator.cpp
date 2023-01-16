#include "parser/declarations/struct-declarator.h"
#include "parser/declarations/declarator.h"
#include "parser/expressions/single-expression.h"

StructDeclarator::StructDeclarator(Node* declarator, Node* expression)
: declarator(declarator), expression(expression) {}

StructDeclarator::~StructDeclarator() {
    delete declarator;
    delete expression;
}

Nodes StructDeclarator::nodes() const {
    if(not declarator) return {expression};
    if(expression) return {declarator, expression};
    return {declarator};
}

Node* StructDeclarator::construct(Parser& parser) {
    if(parser.next().has({"using"})) {
        parser.take();
        return new StructDeclarator(nullptr, SingleExpression::construct(parser));
    }

    Node* declarator = Declarator::construct(parser);
    Node* expression = nullptr;

    if(parser.next().has({"using"})) {
        parser.take();
        expression = SingleExpression::construct(parser);
    }

    return new StructDeclarator(declarator, expression);
}

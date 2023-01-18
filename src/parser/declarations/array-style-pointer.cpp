#include "parser/expressions/single-expression.h"
#include "parser/declarations/array-style-pointer.h"

ArrayStylePointer::ArrayStylePointer(Node* qualifiers, Token* token, Node* expression)
: qualifiers(qualifiers), token(token), expression(expression) {}

ArrayStylePointer::~ArrayStylePointer() {
    delete qualifiers;
    delete expression;
}

Nodes ArrayStylePointer::nodes() const {
    Nodes nodes;

    if(qualifiers) nodes.push_back(qualifiers);
    if(token) nodes.push_back(token);
    if(expression) nodes.push_back(expression);

    return nodes;
}

Node* ArrayStylePointer::construct(Parser &parser, Node *qualifiers) {
    parser.expectingHas({"["});

    Token* token = nullptr;
    Node* expression = nullptr;

    if(parser.next().has({"*"})) token = &parser.take();
    else if(parser.next().has({"<="})) {
        token = &parser.take();
        expression = SingleExpression::construct(parser);
    } else if(not parser.next().has({"]"})) {
        expression = SingleExpression::construct(parser);
    }

    parser.expectingHas({"]"});
    return new ArrayStylePointer(qualifiers, token, expression);
}

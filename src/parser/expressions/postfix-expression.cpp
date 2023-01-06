#include "parser/expressions/primary-expression.h"
#include "parser/expressions/postfix-expression.h"

vector<Component*>  PostfixExpression::nodes() const {
    return {&expression, &op, &identifier};
}

PostfixExpression::PostfixExpression(Node& expression, Token& op, Token& identifier) 
: expression(expression), op(op), identifier(identifier) {}

PostfixExpression::~PostfixExpression() {
    delete &expression;
}

Node* PostfixExpression::construct() {
    Node* node = PrimaryExpression::construct();

    while(parser->next().has(Token::POSTFIX_OPS)) {
        Token& op = parser->take(); 
        Token& identifier = parser->expectingOf({Token::IDENTIFIER});

        node = new PostfixExpression(*node, op, identifier);
    }

    return node;
}

// TODO call expression

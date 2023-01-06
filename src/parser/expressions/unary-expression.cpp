#include "parser/expressions/unary-expression.h"
#include "parser/expressions/postfix-expression.h"

vector<Component*> UnaryExpression::nodes() const {
    return {&op, &expression};
}

UnaryExpression::UnaryExpression(Token& op, Node& expression)
: op(op), expression(expression) {}

UnaryExpression::~UnaryExpression() {
    delete &expression;
}

Node* UnaryExpression::construct() {
    if(parser->next().has(Token::PREFIX_UNARY_OPS)) {
        Token& op = parser->take();
        return new UnaryExpression(op, *UnaryExpression::construct());
    }

    return PostfixExpression::construct();
}

#include "parser/expressions/additive-expression.h"
#include "parser/expressions/multiplicative-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new AdditiveExpression(left, op, right);
}


AdditiveExpression::AdditiveExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* AdditiveExpression::construct() {
    return BinaryOperation::construct(
        {"+", "-"}, 
        MultiplicativeExpression::construct, 
        make);
}

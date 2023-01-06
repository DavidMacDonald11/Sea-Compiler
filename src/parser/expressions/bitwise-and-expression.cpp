#include "parser/expressions/bitwise-and-expression.h"
#include "parser/expressions/shift-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new BitwiseAndExpression(left, op, right);
}


BitwiseAndExpression::BitwiseAndExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* BitwiseAndExpression::construct() {
    return BinaryOperation::construct(
        {"&"},
        ShiftExpression::construct,
        make
    );
}

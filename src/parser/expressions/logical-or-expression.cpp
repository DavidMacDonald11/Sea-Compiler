#include "parser/expressions/logical-or-expression.h"
#include "parser/expressions/logical-xor-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new LogicalOrExpression(left, op, right);
}


LogicalOrExpression::LogicalOrExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* LogicalOrExpression::construct() {
    return BinaryOperation::construct(
        {"or"},
        LogicalXorExpression::construct,
        make
    );
}

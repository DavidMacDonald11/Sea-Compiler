#include "parser/expressions/cast-expression.h"
#include "parser/expressions/comparative-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new ComparativeExpression(left, op, right);
}


ComparativeExpression::ComparativeExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* ComparativeExpression::construct() {
    return BinaryOperation::construct(
        Token::COMPARE_OPS,
        CastExpression::construct,
        make
    );
}

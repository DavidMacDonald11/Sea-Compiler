#include "parser/expressions/bitwise-or-expression.h"
#include "parser/expressions/bitwise-xor-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new BitwiseOrExpression(left, op, right);
}


BitwiseOrExpression::BitwiseOrExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* BitwiseOrExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        {"|"},
        BitwiseXorExpression::construct,
        make
    );
}

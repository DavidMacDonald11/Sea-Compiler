#include "parser/expressions/additive-expression.h"
#include "parser/expressions/shift-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new ShiftExpression(left, op, right);
}


ShiftExpression::ShiftExpression(Node& left, Token& op, Node& right) 
: BinaryOperation(left, op, right) {}

Node* ShiftExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        {"<<", ">>"},
        AdditiveExpression::construct,
        make
    );
}

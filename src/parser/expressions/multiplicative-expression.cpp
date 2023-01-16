#include "parser/expressions/multiplicative-expression.h"
#include "parser/expressions/unary-expression.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new MultiplicativeExpression(left, op, right);
}


MultiplicativeExpression::MultiplicativeExpression(Node& left, Token& op, Node& right) 
: BinaryOperation(left, op, right) {}

Node* MultiplicativeExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        {"*", "/", "%"}, 
        UnaryExpression::construct,
        make);
}

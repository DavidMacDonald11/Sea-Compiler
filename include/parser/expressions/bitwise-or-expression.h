#ifndef BITWISE_OR_EXPRESSION_H
#define BITWISE_OR_EXPRESSION_H

#include "../node.h"

struct BitwiseOrExpression : public BinaryOperation {
    BitwiseOrExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //BITWISE_OR_EXPRESSION_H

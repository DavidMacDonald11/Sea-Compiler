#ifndef BITWISE_AND_EXPRESSION_H
#define BITWISE_AND_EXPRESSION_H

#include "../node.h"

struct BitwiseAndExpression : public BinaryOperation {
    BitwiseAndExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //BITWISE_AND_EXPRESSION_H

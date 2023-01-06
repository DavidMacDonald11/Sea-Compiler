#ifndef BITWISE_XOR_EXPRESSION_H
#define BITWISE_XOR_EXPRESSION_H

#include "../node.h"

struct BitwiseXorExpression : public BinaryOperation {
    BitwiseXorExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //BITWISE_XOR_EXPRESSION_H

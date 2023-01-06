#ifndef SHIFT_EXPRESSION_H
#define SHIFT_EXPRESSION_H

#include "../node.h"

struct ShiftExpression : public BinaryOperation {
    ShiftExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //SHIFT_EXPRESSION_H

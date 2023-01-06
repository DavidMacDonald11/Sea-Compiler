#ifndef COMPARATIVE_EXPRESSION_H
#define COMPARATIVE_EXPRESSION_H

#include "../node.h"

struct ComparativeExpression : public BinaryOperation {
    ComparativeExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //COMPARATIVE_EXPRESSION_H
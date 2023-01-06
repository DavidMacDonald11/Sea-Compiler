#ifndef LOGICAL_OR_EXPRESSION_H
#define LOGICAL_OR_EXPRESSION_H

#include "../node.h"

struct LogicalOrExpression : public BinaryOperation {
    LogicalOrExpression(Node& left, Token& op, Node& right);
    static Node* construct();
};

#endif //LOGICAL_OR_EXPRESSION_H

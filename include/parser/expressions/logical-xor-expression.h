#ifndef LOGICAL_XOR_EXPRESSION_H
#define LOGICAL_XOR_EXPRESSION_H

#include "../node.h"

struct LogicalXorExpression : public BinaryOperation {
    LogicalXorExpression(Node& left, Token& op, Node& right);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //LOGICAL_XOR_EXPRESSION_H

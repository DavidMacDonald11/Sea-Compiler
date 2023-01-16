#ifndef LOGICAL_XOR_EXPRESSION_H
#define LOGICAL_XOR_EXPRESSION_H

#include "../node.h"

struct LogicalXorExpression : public BinaryOperation {
    LogicalXorExpression(Node& left, Token& op, Node& right);
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //LOGICAL_XOR_EXPRESSION_H

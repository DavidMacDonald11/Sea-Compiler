#ifndef LOGICAL_AND_EXPRESSION_H
#define LOGICAL_AND_EXPRESSION_H

#include "../node.h"

struct LogicalAndExpression : public BinaryOperation {
    LogicalAndExpression(Node& left, Token& op, Node& right);
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //LOGICAL_AND_EXPRESSION_H

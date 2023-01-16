#ifndef ADDITIVE_EXPRESSION_H
#define ADDITIVE_EXPRESSION_H

#include "../node.h"

struct AdditiveExpression : public BinaryOperation {
    AdditiveExpression(Node& left, Token& op, Node& right);
    static Node* construct(Parser& parser);
};

#endif //ADDITIVE_EXPRESSION_H

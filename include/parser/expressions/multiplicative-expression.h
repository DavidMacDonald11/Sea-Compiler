#ifndef MULTIPLICATIVE_EXPRESSION_H
#define MULTIPLICATIVE_EXPRESSION_H

#include "../node.h"

struct MultiplicativeExpression : public BinaryOperation {
    MultiplicativeExpression(Node& left, Token& op, Node& right);
    static Node* construct(Parser& parser);
};

#endif //MULTIPLICATIVE_EXPRESSION_H

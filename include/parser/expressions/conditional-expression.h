#ifndef CONDITIONAL_EXPRESSION_H
#define CONDITIONAL_EXPRESSION_H

#include "../node.h"
#include "util/component.h"

struct ConditionalExpression : public Node {
    Node& left;
    Node& condition;
    Node& right;

    vector<Component*> nodes() const override;

    ConditionalExpression(Node& left, Node& condition, Node& right);
    ~ConditionalExpression();

    static Node* construct();
};

#endif //CONDITIONAL_EXPRESSION_H

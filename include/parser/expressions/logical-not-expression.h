#ifndef LOGICAL_NOT_EXPRESSION_H
#define LOGICAL_NOT_EXPRESSION_H

#include "../node.h"

struct LogicalNotExpression : public Node {
    Node& expression;

    vector<Component*> nodes() const override;

    LogicalNotExpression(Node& expression);
    ~LogicalNotExpression();

    static Node* construct();
};

#endif //LOGICAL_NOT_EXPRESSION_H

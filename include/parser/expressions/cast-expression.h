#ifndef CAST_EXPRESSION_H
#define CAST_EXPRESSION_H

#include "../node.h"

struct CastExpression : public Node {
    Node& expression;
    Node& type;

    vector<Component*> nodes() const override;

    CastExpression(Node& expression, Node& type);
    ~CastExpression();

    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //CAST_EXPRESSION_H

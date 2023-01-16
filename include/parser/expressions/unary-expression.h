#ifndef UNARY_EXPRESSION_H
#define UNARY_EXPRESSION_H

#include "../node.h"

struct UnaryExpression : public Node {
    Token& op;
    Node& expression;

    vector<Component*> nodes() const override;

    UnaryExpression(Token& op, Node& expression);
    ~UnaryExpression();

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //UNARY_EXPRESSION_H

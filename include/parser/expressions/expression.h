#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../node.h"

struct Expression : public Node {
    Node& left;
    Node& right;

    vector<Component*> nodes() const override;

    Expression(Node& left, Node& right);
    ~Expression();

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //EXPRESSION_H

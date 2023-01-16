#ifndef COMPARE_STATEMENT_H
#define COMPARE_STATEMENT_H

#include "../node.h"

struct CompareStatement : public Node {
    Node& expression;
    Node* comparator;
    vector<Node*> withs;
    Node* otherwise;

    CompareStatement(Node& expression, Node* comparator, vector<Node*> withs, Node* otherwise);
    ~CompareStatement();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

struct WithStatement : public Node {
    Node& expression;
    Node& statement;

    WithStatement(Node& expression, Node& statement);
    ~WithStatement();

    Nodes nodes() const override;
};

#endif //COMPARE_STATEMENT_H

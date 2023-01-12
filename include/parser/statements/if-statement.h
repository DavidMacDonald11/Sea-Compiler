#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "../node.h"

struct IfStatement : public Node {
    Node& expression;
    Node& statement;
    vector<Node*> others;
    Node* otherwise;

    IfStatement(Node& expression, Node& statement, vector<Node*> others, Node* otherwise);
    ~IfStatement();

    Nodes nodes() const override;
    static Node* construct();
};

struct ElseIf : public Node {
    Node& expression;
    Node& statement;

    ElseIf(Node& expression, Node& statement);
    ~ElseIf();

    Nodes nodes() const override;
};

#endif //IF_STATEMENT_H

#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include "../node.h"

struct WhileStatement : public Node {
    Token* label;
    Node& condition;
    Node& statement;

    WhileStatement(Token* label, Node& condition, Node& statement);
    ~WhileStatement();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //WHILE_STATEMENT_H

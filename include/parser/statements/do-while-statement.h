#ifndef DO_WHILE_STATEMENT_H
#define DO_WHILE_STATEMENT_H

#include "../node.h"

struct DoWhileStatement : public Node {
    Token* label;
    Node& statement;
    Node& condition;

    DoWhileStatement(Token* label, Node& statement, Node& condition);
    ~DoWhileStatement();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //DO_WHILE_STATEMENT_H

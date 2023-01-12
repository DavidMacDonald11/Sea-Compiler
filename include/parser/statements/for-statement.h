#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "../node.h"

struct ForStatement : public Node {
    Token* label;
    Node& declaration;
    Node& condition;
    Node& mod;
    Node& statement;

    ForStatement(Token* label, Node& declaration, Node& condition, Node& mod, Node& statement);
    ~ForStatement();

    Nodes nodes() const override;
    static Node* construct();
private:
    static void takeEndOrKeyword(str keyword);
};

#endif //FOR_STATEMENT_H

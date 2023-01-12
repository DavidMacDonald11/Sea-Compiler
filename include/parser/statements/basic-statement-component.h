#ifndef BASIC_STATEMENT_COMPONENT_H
#define BASIC_STATEMENT_COMPONENT_H

#include "../node.h"

struct BasicStatementComponent : public Node {
    static Node* construct();
};

struct PassStatementComponent : public PrimaryNode {
    PassStatementComponent(Token& token);
    static Node* construct();
};

struct JumpStatementComponent : public Node {
    Token& token;
    Token* label;

    JumpStatementComponent(Token& token, Token* label);

    Nodes nodes() const override;
    static Node* construct();
};

struct ReturnStatementComponent : public Node {
    Token& token;
    Node* expression;

    ReturnStatementComponent(Token& token, Node* expression);
    ~ReturnStatementComponent();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //BASIC_STATEMENT_COMPONENT_H

#ifndef BASIC_STATEMENT_COMPONENT_H
#define BASIC_STATEMENT_COMPONENT_H

#include "../node.h"

struct BasicStatementComponent : public Node {
    static Node* construct(Parser& parser);
};

struct PassStatementComponent : public PrimaryNode {
    PassStatementComponent(Token& token);
    static Node* construct(Parser& parser);
};

struct JumpStatementComponent : public Node {
    Token& token;
    Token* label;

    JumpStatementComponent(Token& token, Token* label);

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

struct ReturnStatementComponent : public Node {
    Token& token;
    Node* expression;

    ReturnStatementComponent(Token& token, Node* expression);
    ~ReturnStatementComponent();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //BASIC_STATEMENT_COMPONENT_H

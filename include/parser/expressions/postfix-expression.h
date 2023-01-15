#ifndef POSTFIX_EXPRESSION_H
#define POSTFIX_EXPRESSION_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct PostfixExpression : public Node {
    Node& expression;
    Token& op;
    Token& identifier;

    Nodes nodes() const override;

    PostfixExpression(Node& expression, Token& op, Token& identifier);
    ~PostfixExpression();

    static Node* construct();
    Transpiler::Line transpile() override;
    Transpiler::Line staticTranspile();
};

struct PostfixIndexExpression : public Node {
    Node* expression;
    Node& index;

    Nodes nodes() const override;

    PostfixIndexExpression(Node& index);
    ~PostfixIndexExpression();

    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //POSTFIX_EXPRESSION_H

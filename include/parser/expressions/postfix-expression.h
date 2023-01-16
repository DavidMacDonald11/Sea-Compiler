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

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
    Transpiler::Line staticTranspile(Transpiler& transpiler);
};

struct PostfixIndexExpression : public Node {
    Node* expression;
    Node& index;

    Nodes nodes() const override;

    PostfixIndexExpression(Node& index);
    ~PostfixIndexExpression();

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //POSTFIX_EXPRESSION_H

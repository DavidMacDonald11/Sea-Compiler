#ifndef PRIMARY_EXPRESSION_H
#define PRIMARY_EXPRESSION_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct PrimaryExpression : public Node {
    static Node* construct();
};

struct NumericConstant : public PrimaryNode {
    NumericConstant(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct Identifier : public PrimaryNode {
    Identifier(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct ParentheseseExpression : public Node {
    Node& expression;

    vector<Component*> nodes() const override;

    ParentheseseExpression(Node& expression);

    static Node* construct();
    Transpiler::Line transpile() override;
};

struct PrimaryKeyword : public PrimaryNode {
    PrimaryKeyword(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //PRIMARY_EXPRESSION_H

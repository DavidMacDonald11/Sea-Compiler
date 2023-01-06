#ifndef PRIMARY_EXPRESSION_H
#define PRIMARY_EXPRESSION_H

#include "../node.h"

struct PrimaryExpression : public Node {
    static Node* construct();
};

struct NumericConstant : public PrimaryNode {
    NumericConstant(Token& token);
    static Node* construct();
};

struct Identifier : public PrimaryNode {
    Identifier(Token& token);
    static Node* construct();
};

struct ParentheseseExpression : public Node {
    static Node* construct();
};

struct PrimaryKeyword : public PrimaryNode {
    PrimaryKeyword(Token& token);
    static Node* construct();
};

#endif //PRIMARY_EXPRESSION_H
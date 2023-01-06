#ifndef PRIMARY_EXPRESSION_H
#define PRIMARY_EXPRESSION_H

#include "../node.h"

class PrimaryExpression : public Node {
public:
    static Node* construct();
};

class NumericConstant : public PrimaryNode {
public:
    NumericConstant(Token& token);

    static Node* construct();
};

class Identifier : public PrimaryNode {
public:
    Identifier(Token& token);

    static Node* construct();
};

class PrimaryKeyword : public PrimaryNode {
public:
    PrimaryKeyword(Token& token);

    static Node* construct();
};

#endif //PRIMARY_EXPRESSION_H

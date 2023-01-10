#ifndef ALLOCATION_EXPRESSION_H
#define ALLOCATION_EXPRESSION_H

#include "../node.h"

struct AllocationExpression : public Node {
    static Node* construct();
};

struct AllocExpression : public Node {
    Node& expression;
    Token* token;

    AllocExpression(Node& expression, Token* token);
    ~AllocExpression();

    Nodes nodes() const override;
    static Node* construct();
};

struct ReallocExpression : public Node {
    Node& expression;
    Node& size;

    ReallocExpression(Node& expression, Node& size);
    ~ReallocExpression();

    Nodes nodes() const override;
    static Node* construct();
};

struct UnallocExpression : public Node {
    Node& expression;

    UnallocExpression(Node& expression);
    ~UnallocExpression();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //ALLOCATION_EXPRESSION_H

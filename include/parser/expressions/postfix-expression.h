#ifndef POSTFIX_EXPRESSION_H
#define POSTFIX_EXPRESSION_H

#include "../node.h"

struct PostfixExpression : public Node {
    Node& expression;
    Token& op;
    Token& identifier;

    vector<Component*> nodes() const override;

    PostfixExpression(Node& expression, Token& op, Token& identifier);
    ~PostfixExpression();

    static Node* construct();
};

#endif //POSTFIX_EXPRESSION_H

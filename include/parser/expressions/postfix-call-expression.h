#ifndef POSTFIX_CALL_EXPRESSION_H
#define POSTFIX_CALL_EXPRESSION_H

#include "../node.h"

struct PostfixCallExpression : public Node {
    Node* expression;
    vector<Node*> parameters;
    vector<Node*> defaults;

    Nodes nodes() const override;

    PostfixCallExpression(vector<Node*> parameters, vector<Node*> defaults);
    ~PostfixCallExpression();

    static Node* construct(Parser& parser);
};

struct DefaultArgument : public Node {
    Token& identifier;
    Node& expression;

    Nodes nodes() const override;

    DefaultArgument(Token& identifier, Node& expression);
    ~DefaultArgument();

    static Node* construct(Parser& parser);
};

#endif //POSTFIX_CALL_EXPRESSION_H

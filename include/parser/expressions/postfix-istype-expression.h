#ifndef POSTFIX_ISTYPE_EXPRESSION_H
#define POSTFIX_ISTYPE_EXPRESSION_H

#include "../node.h"

struct PostfixIstypeExpression : public Node {
    Node* expression;
    vector<Node*> associations;
    Node* otherwise;

    PostfixIstypeExpression(vector<Node*> associations, Node* otherwise);
    ~PostfixIstypeExpression();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //POSTFIX_ISTYPE_EXPRESSION_H

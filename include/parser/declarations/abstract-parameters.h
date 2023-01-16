#ifndef ABSTRACT_PARAMETERS_H
#define ABSTRACT_PARAMETERS_H

#include "../node.h"

struct AbstractParameters : public Node {
    vector<Node*> parameters;
    Token* varargs;

    AbstractParameters(vector<Node*> parameters, Token* varargs);
    ~AbstractParameters();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //ABSTRACT_PARAMETERS_H

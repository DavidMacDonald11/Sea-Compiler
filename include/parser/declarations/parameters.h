#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "../node.h"

struct Parameters : public Node {
    vector<Node*> parameters;
    vector<Node*> defaults;
    Token* varargs;

    Parameters(vector<Node*> parameters, vector<Node*> defaults, Token* varargs);
    ~Parameters();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //PARAMETERS_H

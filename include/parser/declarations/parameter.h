#ifndef PARAMETER_H
#define PARAMETER_H

#include "../node.h"

struct Parameter : public Node {
    Node& specifiers;
    Token& identifier;

    Parameter(Node& specifiers, Token& identifier);
    ~Parameter();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //PARAMETER_H

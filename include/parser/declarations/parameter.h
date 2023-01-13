#ifndef PARAMETER_H
#define PARAMETER_H

#include "../node.h"

struct Parameter : public Node {
    Node& specifiers;
    Node* declarator;

    Parameter(Node& specifiers, Node* declarator);
    ~Parameter();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //PARAMETER_H

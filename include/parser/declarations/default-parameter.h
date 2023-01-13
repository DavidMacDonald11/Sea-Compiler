#ifndef DEFAULT_PARAMETER_H
#define DEFAULT_PARAMETER_H

#include "../node.h"

struct DefaultParameter : public Node {
    Node& parameter;
    Node& expression;

    DefaultParameter(Node& parameter, Node& expression);
    ~DefaultParameter();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //DEFAULT_PARAMETER_H

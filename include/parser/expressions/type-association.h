#ifndef TYPE_ASSOCIATION_H
#define TYPE_ASSOCIATION_H

#include "../node.h"

struct TypeAssociation : public Node {
    Node& type;
    Node& expression;

    TypeAssociation(Node& type, Node& expression);
    ~TypeAssociation();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //TYPE_ASSOCIATION_H

#ifndef QUALIFIER_TYPE_SPECIFIER_H
#define QUALIFIER_TYPE_SPECIFIER_H

#include "../node.h"

struct QualifierTypeSpecifier : public Node {
    Node& qualifier;
    Node& type;

    QualifierTypeSpecifier(Node& qualifier, Node& type);
    ~QualifierTypeSpecifier();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //QUALIFIER_TYPE_SPECIFIER_H

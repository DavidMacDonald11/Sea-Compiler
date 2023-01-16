#ifndef SPECIFIER_QUALIFIER_LIST_H
#define SPECIFIER_QUALIFIER_LIST_H

#include "../node.h"

struct SpecifierQualifierList : public Node {
    Node* qualifiers;
    Node* align;
    Node& type;

    vector<Component*> nodes() const override;

    SpecifierQualifierList(Node* qualifiers, Node* align, Node& type);
    ~SpecifierQualifierList();

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //SPECIFIER_QUALIFIER_LIST_H

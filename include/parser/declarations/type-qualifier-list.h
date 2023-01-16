#ifndef TYPE_QUALIFIER_LIST_H
#define TYPE_QUALIFIER_LIST_H

#include "../node.h"

struct TypeQualifierList : public Node {
    vector<Node*> qualifiers;

    vector<Component*> nodes() const override;

    TypeQualifierList(vector<Node*> qualifiers);
    ~TypeQualifierList();

    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //TYPE_QUALIFIER_LIST_H

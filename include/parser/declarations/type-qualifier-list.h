#ifndef TYPE_QUALIFIER_LIST_H
#define TYPE_QUALIFIER_LIST_H

#include "../node.h"

struct TypeQualifierList : public Node {
    vector<Node*> qualifiers;

    TypeQualifierList(vector<Node*> qualifiers);
    ~TypeQualifierList();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //TYPE_QUALIFIER_LIST_H

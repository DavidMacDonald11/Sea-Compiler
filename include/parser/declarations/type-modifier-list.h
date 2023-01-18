#ifndef TYPE_MODIFIER_LIST_H
#define TYPE_MODIFIER_LIST_H

#include "../node.h"

struct TypeModifierList : public Node {
    Node* qualifiers;
    Node* align;

    TypeModifierList(Node* qualifiers, Node* align);
    ~TypeModifierList();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //TYPE_MODIFIER_LIST_H

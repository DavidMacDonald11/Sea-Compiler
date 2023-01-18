#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include "../node.h"

struct TypeName : public Node {
    Node* list;
    Node& type;
    Node* pointers;

    TypeName(Node* list, Node& type, Node* pointers);
    ~TypeName();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //TYPE_NAME_H

#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include "../node.h"

struct TypeName : public Node {
    Node& list;
    Node* declarator;

    TypeName(Node& list, Node* declarator);
    ~TypeName();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //TYPE_NAME_H
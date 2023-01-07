#ifndef TYPE_NAME_H
#define TYPE_NAME_H

#include "../node.h"

struct TypeName : public Node {
    Node& list;
    //Node& declarator

    vector<Component*> nodes() const override;

    TypeName(Node& list);
    ~TypeName();

    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //TYPE_NAME_H

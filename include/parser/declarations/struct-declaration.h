#ifndef STRUCT_DECLARATION_H
#define STRUCT_DECLARATION_H

#include "../node.h"

struct StructDeclaration : public Node {
    Node& list;
    vector<Node*> declarators;

    StructDeclaration(Node& list, vector<Node*> declarators);
    ~StructDeclaration();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //STRUCT_DECLARATION_H

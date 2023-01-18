#ifndef STRUCT_DECLARATION_H
#define STRUCT_DECLARATION_H

#include "../node.h"

struct StructDeclaration : public Node {
    Node& type;
    vector<Node*> declarators;

    StructDeclaration(Node& type, vector<Node*> declarators);
    ~StructDeclaration();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //STRUCT_DECLARATION_H

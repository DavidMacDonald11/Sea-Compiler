#ifndef STRUCT_DECLARATOR_H
#define STRUCT_DECLARATOR_H

#include "../node.h"

struct StructDeclarator : public Node {
    Token* identifier;
    Node* expression;

    StructDeclarator(Token* identifier, Node* expression);
    ~StructDeclarator();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //STRUCT_DECLARATOR_H

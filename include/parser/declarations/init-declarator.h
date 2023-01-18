#ifndef INIT_DECLARATOR_H
#define INIT_DECLARATOR_H

#include "../node.h"

struct InitDeclarator : public Node {
    Token& identifier;
    Node* expression;

    InitDeclarator(Token& identifier, Node* expression);
    ~InitDeclarator();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //INIT_DECLARATOR_H

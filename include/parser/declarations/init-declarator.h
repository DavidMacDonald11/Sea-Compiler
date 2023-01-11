#ifndef INIT_DECLARATOR_H
#define INIT_DECLARATOR_H

#include "../node.h"

struct InitDeclarator : public Node {
    Node& declarator;
    Node* expression;

    InitDeclarator(Node& declarator, Node* expression);
    ~InitDeclarator();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //INIT_DECLARATOR_H
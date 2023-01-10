#ifndef ARRAY_DECLARATOR_H
#define ARRAY_DECLARATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct ArrayDeclarator : public Node {
    Node* qualifiers;
    Token* token;
    Node* expression;

    ArrayDeclarator(Node* qualifiers, Token* token, Node* expression);
    ~ArrayDeclarator();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //ARRAY_DECLARATOR_H

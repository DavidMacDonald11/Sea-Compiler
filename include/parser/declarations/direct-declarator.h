#ifndef DIRECT_DECLARATOR_H
#define DIRECT_DECLARATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct DirectDeclarator : public Node {
    Token* identifier;
    Node* declarator;
    Node* array;

    DirectDeclarator(Token* identifier, Node* declarator, Node* array);
    ~DirectDeclarator();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //DIRECT_DECLARATOR_H

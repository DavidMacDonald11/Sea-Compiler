#ifndef POINTER_H
#define POINTER_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct Pointers : public Node {
    Node* qualifiers;
    Token& token;
    Node* pointer;

    Pointers(Node* qualifiers, Token& token, Node* pointer);
    ~Pointers();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //POINTER_H

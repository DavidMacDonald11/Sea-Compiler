#ifndef POINTER_H
#define POINTER_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct Pointer : public Node {
    Token& token;
    Node* qualifiers;
    Node* pointer;

    Pointer(Token& token, Node* qualifiers, Node* pointer);
    ~Pointer();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Publisher::Value* publish(Publisher& publisher) override;
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //POINTER_H

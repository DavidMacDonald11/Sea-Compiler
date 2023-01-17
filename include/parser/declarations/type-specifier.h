#ifndef TYPE_SPECIFIER_H
#define TYPE_SPECIFIER_H

#include "../node.h"

struct TypeSpecifier : public Node {
    Token* token;
    Node* node;

    TypeSpecifier(Token* token, Node* node = nullptr);
    ~TypeSpecifier();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Publisher::Value* publish(Publisher& publisher) override;
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //TYPE_SPECIFIER_H

#ifndef ATOMIC_TYPE_SPECIFIER_H
#define ATOMIC_TYPE_SPECIFIER_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct AtomicTypeSpecifier : public Node {
    Node& type;

    AtomicTypeSpecifier(Node& type);
    ~AtomicTypeSpecifier();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //ATOMIC_TYPE_SPECIFIER_H

#ifndef ALIGN_SPECIFIER_H
#define ALIGN_SPECIFIER_H

#include "../node.h"

struct AlignSpecifier : public Node {
    Node& alignOf;

    AlignSpecifier(Node& alignOf);
    ~AlignSpecifier();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //ALIGN_SPECIFIER_H

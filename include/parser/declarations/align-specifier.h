#ifndef ALIGN_SPECIFIER_H
#define ALIGN_SPECIFIER_H

#include "../node.h"

struct AlignSpecifier : public Node {
    Node& alignOf;

    vector<Component*> nodes() const override;

    AlignSpecifier(Node& alignOf);
    ~AlignSpecifier();

    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //ALIGN_SPECIFIER_H
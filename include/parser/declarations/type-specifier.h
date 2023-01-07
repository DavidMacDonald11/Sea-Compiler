#ifndef TYPE_SPECIFIER_H
#define TYPE_SPECIFIER_H

#include "../node.h"

struct TypeSpecifier : public PrimaryNode {
    TypeSpecifier(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //TYPE_SPECIFIER_H

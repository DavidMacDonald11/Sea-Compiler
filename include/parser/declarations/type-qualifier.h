#ifndef TYPE_QUALIFIER_H
#define TYPE_QUALIFIER_H

#include "../node.h"

struct TypeQualifier : public PrimaryNode {
    TypeQualifier(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //TYPE_QUALIFIER_H

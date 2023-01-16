#ifndef TYPE_QUALIFIER_H
#define TYPE_QUALIFIER_H

#include "../node.h"

struct TypeQualifier : public PrimaryNode {
    TypeQualifier(Token& token);
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //TYPE_QUALIFIER_H

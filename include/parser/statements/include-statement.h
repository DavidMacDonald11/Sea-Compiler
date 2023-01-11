#ifndef INCLUDE_STATEMENT_H
#define INCLUDE_STATEMENT_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct IncludeStatement : public PrimaryNode {
    IncludeStatement(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //INCLUDE_STATEMENT_H

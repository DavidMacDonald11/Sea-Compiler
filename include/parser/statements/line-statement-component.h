#ifndef LINE_STATEMENT_COMPONENT_H
#define LINE_STATEMENT_COMPONENT_H

#include "hidden-statement.h"
#include "transpiler/transpiler.h"

struct LineStatementComponent : public HiddenStatement {
    LineStatementComponent(Node& statement);
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //LINE_STATEMENT_COMPONENT_H

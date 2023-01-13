#ifndef STATEMENT_H
#define STATEMENT_H

#include "hidden-statement.h"
#include "transpiler/transpiler.h"

struct Statement : public HiddenStatement {
    Statement(Node& statement);
    static Node* construct();
    Transpiler::Line transpile() override;

    static Node* newLineStatement();
};

#endif //STATEMENT_H

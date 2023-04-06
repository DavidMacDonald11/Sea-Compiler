#ifndef STATEMENT_H
#define STATEMENT_H

#include "hidden-statement.h"
#include "transpiler/transpiler.h"

struct Statement : public HiddenStatement {
    Statement(Node& statement);
    static Node* construct(Parser& parser);
    Publisher::Value* publish(Publisher& publisher) override;
    Transpiler::Line transpile(Transpiler& transpiler) override;

    static Node* newLineStatement(Parser& parser);
};

#endif //STATEMENT_H

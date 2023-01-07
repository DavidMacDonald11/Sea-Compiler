#ifndef COMPOUND_STATEMENT_H
#define COMPOUND_STATEMENT_H

#include "../node.h"

struct CompoundStatement : public Node {
    vector<Node*> statements;

    vector<Component*> nodes() const override;

    CompoundStatement(vector<Node*> statements);
    ~CompoundStatement();

    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //COMPOUND_STATEMENT_H

#ifndef HIDDEN_STATEMENT_H
#define HIDDEN_STATEMENT_H

#include "../node.h"

struct HiddenStatement : public Node {
    Node& statement;

    virtual vector<Component*> nodes() const override;
    virtual str tree(str prefix) const override;

    HiddenStatement(Node& statement);
    ~HiddenStatement();

    virtual Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //HIDDEN_STATEMENT_H

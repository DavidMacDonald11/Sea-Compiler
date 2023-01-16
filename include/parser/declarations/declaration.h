#ifndef DECLARATION_H
#define DECLARATION_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct Declaration : public Node {
    Node* specifiers;
    vector<Node*> declarators;

    Declaration(Node* specifiers, vector<Node*> declarators);
    ~Declaration();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //DECLARATION_H

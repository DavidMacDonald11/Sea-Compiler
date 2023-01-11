#ifndef DESIGNATOR_H
#define DESIGNATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct Designator : public Node {
    Token& token;
    Node* expression;
    Token* identifier;

    Designator(Token& token, Node* expression, Token* identifier);
    ~Designator();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //DESIGNATOR_H

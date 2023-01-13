#ifndef FUNCTION_DEFINITION_H
#define FUNCTION_DEFINITION_H

#include "../node.h"

struct FunctionDefinition : public Node {
    Node* specifiers;
    Token& name;
    Node* parameters;
    Node* type;
    Node& statement;

    FunctionDefinition(Node* specifiers, Token& name, Node* params, Node* type, Node& statement);
    ~FunctionDefinition();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //FUNCTION_DEFINITION_H

#ifndef FUNCTION_TYPE_SPECIFIER_H
#define FUNCTION_TYPE_SPECIFIER_H

#include "../node.h"

struct FunctionTypeSpecifier : public Node {
    Token* async;
    Token& fun;
    Node* parameters;
    Node* type;

    FunctionTypeSpecifier(Token* async, Token& fun ,Node* parameters, Node* type);
    ~FunctionTypeSpecifier();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //FUNCTION_TYPE_SPECIFIER_H

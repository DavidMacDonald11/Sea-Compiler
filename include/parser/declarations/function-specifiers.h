#ifndef FUNCTION_SPECIFIERS_H
#define FUNCTION_SPECIFIERS_H

#include "../node.h"

struct FunctionSpecifiers : public Node {
    Token* visibility;
    vector<Token*> tokens;

    FunctionSpecifiers(Token* visibility, vector<Token*> tokens);

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //FUNCTION_SPECIFIERS_H

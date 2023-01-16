#ifndef C_STATEMENT_H
#define C_STATEMENT_H

#include "../node.h"

struct CStatement : public Node {
    Token& token;
    Token* cTokens;

    CStatement(Token& token, Token* cTokens = nullptr);
    
    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //C_STATEMENT_H

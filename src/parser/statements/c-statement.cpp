#include "parser/statements/c-statement.h"
#include "transpiler/transpiler.h"

CStatement::CStatement(Token& token, Token* cTokens) 
: token(token), cTokens(cTokens) {}

Nodes CStatement::nodes() const {
    if(cTokens) return {&token, cTokens};
    return {&token};
}

Node* CStatement::construct(Parser& parser) {
    if(not parser.next().has({"###"})) return nullptr;

    Token& token = parser.take();

    if(parser.next().has({"\n"})) {
        parser.take();
        return new CStatement(token);
    }

    Token* cTokens = &parser.expectingOf({Token::C_TOKENS});
    parser.expectingHas({"\n"});

    return new CStatement(token, cTokens);
}

Transpiler::Line CStatement::transpile(Transpiler&) {
    if(cTokens) return {"", cTokens->string + "\n"};
    return {};
}

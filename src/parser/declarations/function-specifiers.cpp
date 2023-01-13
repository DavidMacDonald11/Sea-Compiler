#include "parser/declarations/function-specifiers.h"

FunctionSpecifiers::FunctionSpecifiers(Token* visibility, vector<Token*> tokens)
: visibility(visibility), tokens(tokens) {}

Nodes FunctionSpecifiers::nodes() const {
    Nodes nodes;
    for(Token* token : tokens) nodes.push_back(token);
    return nodes;
}

Node* FunctionSpecifiers::construct() {
    Token* visibility = nullptr; 
    if(parser->next().has(Token::VISIBILITY_KEYWORDS)) visibility = &parser->take();

    vector<Token*> tokens;
    while(parser->next().has(Token::FUNCTION_KEYWORDS)) tokens.push_back(&parser->take());
    
    if(not visibility and tokens.size() == 0) return nullptr;
    return new FunctionSpecifiers(visibility, tokens);
}

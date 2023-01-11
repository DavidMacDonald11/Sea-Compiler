#include "parser/statements/partial-import.h"

PartialImport::PartialImport(Token& import, Token* rename) 
: import(import), rename(rename) {}

Nodes PartialImport::nodes() const {
    Nodes nodes = {&import};
    if(rename) nodes.push_back(rename);
    return nodes;
}

Node* PartialImport::construct() {
    parser->expectingHas({"import"});
    Token& import = parser->expectingOf({Token::IDENTIFIER});

    if(not parser->next().has({"as"})) return new PartialImport(import);
    
    parser->take();
    Token* rename = &parser->expectingOf({Token::IDENTIFIER});
    return new PartialImport(import, rename);
}

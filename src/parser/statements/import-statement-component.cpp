#include "parser/statements/import-statement-component.h"
#include "parser/parser.h"
#include "parser/statements/partial-import.h"

ImportStatementComponent::ImportStatementComponent(Token& file, Token* rename) 
: file(file), rename(rename) {}

Nodes ImportStatementComponent::nodes() const {
    Nodes nodes = {&file};
    if(rename) nodes.push_back(rename);
    return nodes;
}

Node* ImportStatementComponent::construct() {
    if(parser->next().has({"from"})) return FromImportStatementComponent::construct();
    if(not parser->next().has({"import"})) return nullptr;
    
    parser->take();
    Token& file = parser->expectingOf({Token::STR});

    if(not parser->next().has({"as"})) return new ImportStatementComponent(file);
    
    parser->take();
    Token* rename = &parser->expectingOf({Token::IDENTIFIER});
    return new ImportStatementComponent(file, rename);
}


FromImportStatementComponent::FromImportStatementComponent(Token& file, vector<Node*> imports)
: file(file), imports(imports), all(nullptr) {}

FromImportStatementComponent::FromImportStatementComponent(Token& file, Token* all)
: file(file), imports(), all(all) {}

FromImportStatementComponent::~FromImportStatementComponent() {
    for(Node* node : imports) delete node;
}

Nodes FromImportStatementComponent::nodes() const {
    Nodes nodes = {&file};
    for(Node* node : imports) nodes.push_back(node);
    if(all) nodes.push_back(all);

    return nodes;
}

Node* FromImportStatementComponent::construct() {
    parser->expectingHas({"from"});
    Token& file = parser->expectingOf({Token::STR});

    parser->expectingHas({"import"});

    if(parser->next().has({"*"})) 
        return new FromImportStatementComponent(file, &parser->take());
    
    parser->i -= 1;
    vector<Node*> nodes;

    while(true) {
        nodes.push_back(PartialImport::construct());
        if(not parser->next().has({","})) break;
        parser->take();
        parser->skipNewlines();
    }

    return new FromImportStatementComponent(file, nodes);
}

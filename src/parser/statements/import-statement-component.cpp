#include "parser/statements/import-statement-component.h"
#include "parser/parser.h"
#include "parser/statements/partial-import.h"

ImportStatementComponent::ImportStatementComponent(Token* lib, Token& file, Token* rename) 
: lib(lib), file(file), rename(rename) {}

Nodes ImportStatementComponent::nodes() const {
    Nodes nodes;
    
    if(lib) nodes.push_back(lib);
    nodes.push_back(&file);
    if(rename) nodes.push_back(rename);

    return nodes;
}

Node* ImportStatementComponent::construct(Parser& parser) {
    if(parser.next().has({"from"})) return FromImportStatementComponent::construct(parser);
    if(not parser.next().has({"import"})) return nullptr;
    parser.take();

    Token* lib = (parser.next().has({"lib"}))? &parser.take() : nullptr;
    Token& file = parser.expectingOf({Token::STR});

    if(not parser.next().has({"as"})) return new ImportStatementComponent(lib, file);
    
    parser.take();
    Token* rename = &parser.expectingOf({Token::IDENTIFIER});
    return new ImportStatementComponent(lib, file, rename);
}


FromImportStatementComponent::FromImportStatementComponent(Token* lib, Token& file, vector<Node*> imports)
: lib(lib), file(file), imports(imports), all(nullptr) {}

FromImportStatementComponent::FromImportStatementComponent(Token* lib, Token& file, Token* all)
: lib(lib), file(file), imports(), all(all) {}

FromImportStatementComponent::~FromImportStatementComponent() {
    for(Node* node : imports) delete node;
}

Nodes FromImportStatementComponent::nodes() const {
    Nodes nodes;
    nodes.reserve(1 + imports.size());

    if(lib) nodes.push_back(lib);
    nodes.push_back(&file);
    for(Node* node : imports) nodes.push_back(node);
    if(all) nodes.push_back(all);

    return nodes;
}

Node* FromImportStatementComponent::construct(Parser& parser) {
    parser.expectingHas({"from"});

    Token* lib = (parser.next().has({"lib"}))? &parser.take() : nullptr;
    Token& file = parser.expectingOf({Token::STR});

    if(parser.ahead(1).has({"*"})) {
        parser.expectingHas({"import"});
        return new FromImportStatementComponent(lib, file, &parser.take());
    }
        
    vector<Node*> nodes;

    while(true) {
        nodes.push_back(PartialImport::construct(parser));
        if(not parser.next().has({","})) break;
        parser.take();
        parser.skipNewlines();
    }

    return new FromImportStatementComponent(lib, file, nodes);
}

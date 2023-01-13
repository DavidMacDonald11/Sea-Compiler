#include "parser/declarations/struct-declaration.h"
#include "parser/declarations/assert-declaration.h"
#include "parser/declarations/specifier-qualifier-list.h"
#include "parser/declarations/struct-declarator.h"

StructDeclaration::StructDeclaration(Node& list, vector<Node*> declarators)
: list(list), declarators(declarators) {}

StructDeclaration::~StructDeclaration() {
    delete &list;
    for(Node* node : declarators) delete node;
}

Nodes StructDeclaration::nodes() const {
    Nodes nodes;
    nodes.reserve(declarators.size() + 1);

    for(Node* node : declarators) nodes.push_back(node);
    return nodes;
}

Node* StructDeclaration::construct() {
    if(parser->nextOrAfterHas({"assert"})) {
        Node* node =  AssertDeclaration::construct();
        parser->expectingHas(Token::LINE_ENDS);
        return node;
    }

    Node& list = *SpecifierQualifierList::construct();
    vector<Node*> declarators;

    while(not parser->next().has(Token::LINE_ENDS)) {
        declarators.push_back(StructDeclarator::construct());
        if(not parser->next().has({","})) break;

        parser->take();
        parser->skipNewlines();
    }

    parser->expectingHas(Token::LINE_ENDS);
    parser->skipNewlines();
    
    return new StructDeclaration(list, declarators);
}

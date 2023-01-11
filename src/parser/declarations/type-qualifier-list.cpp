#include "parser/declarations/type-qualifier.h"
#include "parser/declarations/type-qualifier-list.h"

vector<Component*> TypeQualifierList::nodes() const {
    vector<Component*> nodes;
    for(Node* node : qualifiers) nodes.push_back(node);
    return nodes;
}

TypeQualifierList::TypeQualifierList(vector<Node*> qualifiers)
: qualifiers(qualifiers) {}

TypeQualifierList::~TypeQualifierList() {
    for(Node* node : qualifiers) delete node;
}

Node* TypeQualifierList::construct() {
    vector<Node*> nodes;

    while(parser->next().has(Token::TYPE_QUALIFIER_KEYWORDS)) {
        if(parser->next().has({"atomic"}) and parser->ahead(1).has({"("})) 
            break;
        
        Node* node = TypeQualifier::construct();
        nodes.push_back(node);
    } 

    if(nodes.size() == 0) return nullptr;
    return new TypeQualifierList(nodes);
}

Transpiler::Line TypeQualifierList::transpile() {
    Transpiler::Line result;

    for(Node* node : qualifiers) 
        result.add("", fmt::format("{} ", node->transpile().toString()));

    result.string.pop_back();
    return result;
}

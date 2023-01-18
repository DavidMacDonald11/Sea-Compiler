#include "parser/declarations/type-qualifier.h"
#include "parser/declarations/type-qualifier-list.h"
#include "publisher/publisher.h"

TypeQualifierList::TypeQualifierList(vector<Node*> qualifiers)
: qualifiers(qualifiers) {}

TypeQualifierList::~TypeQualifierList() {
    for(Node* node : qualifiers) delete node;
}

Nodes TypeQualifierList::nodes() const {
    vector<Component*> nodes;
    for(Node* node : qualifiers) nodes.push_back(node);
    return nodes;
}

Node* TypeQualifierList::construct(Parser& parser) {
    vector<Node*> nodes;

    while(parser.next().has(Token::TYPE_QUALIFIER_KEYWORDS)) {
        if(parser.next().has({"atomic"}) and parser.ahead(1).has({"("})) 
            break;
        
        Node* node = TypeQualifier::construct(parser);
        nodes.push_back(node);
    } 

    if(nodes.size() == 0) return nullptr;
    return new TypeQualifierList(nodes);
}

Transpiler::Line TypeQualifierList::transpile(Transpiler& transpiler) {
    Transpiler::Line result;

    for(Node* node : qualifiers) 
        result.add("", node->transpile(transpiler).toString() + " ");

    result.string.pop_back();
    return result;
}

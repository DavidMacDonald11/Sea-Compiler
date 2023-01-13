#include "parser/expressions/postfix-istype-expression.h"
#include "parser/expressions/single-expression.h"
#include "parser/expressions/type-association.h"

PostfixIstypeExpression::PostfixIstypeExpression(vector<Node*> associations, Node* otherwise)
: expression(nullptr), associations(associations), otherwise(otherwise) {}

PostfixIstypeExpression::~PostfixIstypeExpression() {
    delete expression;
    for(Node* node : associations) delete node;
    delete otherwise;
}

Nodes PostfixIstypeExpression::nodes() const {
    Nodes nodes = {expression};
    nodes.reserve(associations.size());

    for(Node* node : associations) nodes.push_back(node);
    if(otherwise) nodes.push_back(otherwise);

    return nodes;
}

Node* PostfixIstypeExpression::construct() {
    parser->expectingHas({"("});
    parser->skipNewlines();

    vector<Node*> associations;
    Node* otherwise = nullptr;

    while(not parser->next().has({")"})) {
        if(parser->next().has({"else"})) {
            parser->take();
            otherwise = SingleExpression::construct();
            parser->skipNewlines();
            break;
        }

        associations.push_back(TypeAssociation::construct());
        
        if(not parser->next().has({","})) break;
        parser->take();
        parser->skipNewlines();
    }

    parser->expectingHas({")"});
    return new PostfixIstypeExpression(associations, otherwise);
}

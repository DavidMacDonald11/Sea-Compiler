#include "parser/declarations/abstract-parameters.h"
#include "parser/declarations/parameter.h"

AbstractParameters::AbstractParameters(vector<Node*> parameters, Token* varargs) 
: parameters(parameters), varargs(varargs) {}

AbstractParameters::~AbstractParameters() {
    for(Node* node : parameters) delete node;
}

Nodes AbstractParameters::nodes() const {
    Nodes nodes;
    nodes.reserve(parameters.size());

    for(Node* node : parameters) nodes.push_back(node);
    if(varargs) nodes.push_back(varargs);

    return nodes;
}

Node* AbstractParameters::construct() {
    vector<Node*> nodes;
    Token* varargs = nullptr;

    nodes.push_back(Parameter::construct());

    while(parser->next().has({","})) {
        parser->take();

        if(parser->next().has({"..."})) {
            varargs = &parser->take();
            break;
        }

        nodes.push_back(Parameter::construct());
    }

    return new AbstractParameters(nodes, varargs);
}

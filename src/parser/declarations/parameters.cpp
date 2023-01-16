#include "parser/declarations/parameters.h"
#include "parser/declarations/default-parameter.h"

Parameters::Parameters(vector<Node*> parameters, vector<Node*> defaults, Token* varargs)
: parameters(parameters), defaults(defaults), varargs(varargs) {}

Parameters::~Parameters() {
    for(Node* node : parameters) delete node;
    for(Node* node : defaults) delete node;
}

Nodes Parameters::nodes() const {
    Nodes nodes;
    nodes.reserve(parameters.size() + defaults.size());

    for(Node* node : parameters) nodes.push_back(node);
    for(Node* node : defaults) nodes.push_back(node);
    if(varargs) nodes.push_back(varargs);

    return nodes;
}

Node* Parameters::construct(Parser& parser) {
    vector<Node*> parameters;
    vector<Node*> defaults;
    Token* varargs = nullptr;

    parser.context.allowDirectAbstractDeclarator = true;

    while(not parser.next().has({")"})) {
        Node* node = DefaultParameter::construct(parser);
        bool isDefault = className(node) == "DefaultParameter";

        if(defaults.size() > 0 and not isDefault) {
            parser.fault.error(*node, "Non-default parameters must precede default parameters.");
        }

        (isDefault? &defaults : &parameters)->push_back(node);
        
        if(parser.next().has({")"})) break;
        parser.expectingHas({","}); 

        if(parser.next().has({"..."})) {
            varargs = &parser.take();
            break;
        }
    }

    parser.context.allowDirectAbstractDeclarator = false;
    return new Parameters(parameters, defaults, varargs);
}

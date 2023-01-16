#include "parser/expressions/postfix-call-expression.h"
#include "parser/expressions/single-expression.h"

Nodes PostfixCallExpression::nodes() const {
    Nodes nodes = {expression};
    nodes.reserve(parameters.size() + defaults.size());

    for(Node* node : parameters) nodes.push_back(node);
    for(Node* node : defaults) nodes.push_back(node);

    return nodes;
}

PostfixCallExpression::PostfixCallExpression(vector<Node*> parameters, vector<Node*> defaults) 
: expression(nullptr), parameters(parameters), defaults(defaults) {}

PostfixCallExpression::~PostfixCallExpression() {
    delete expression;
    for(Node* node : parameters) delete node;
    for(Node* node : defaults) delete node;
}

Node* PostfixCallExpression::construct(Parser& parser) {
    vector<Node*> parameters;
    vector<Node*> defaults;

    while(not parser.next().has({")"})) {
        Node* node = DefaultArgument::construct(parser);
        bool isDefault = className(node) == "DefaultArgument";

        if(defaults.size() > 0 and not isDefault) {
            parser.fault.error(*node, "Parameter arguments must precede default arguments.");
        }

        (isDefault? &defaults : &parameters)->push_back(node);
        
        if(parser.next().has({")"})) break;
        parser.expectingHas({","}); 
    }

    parser.expectingHas({")"});
    return new PostfixCallExpression(parameters, defaults);
}


Nodes DefaultArgument::nodes() const {
    return {&identifier, &expression};
}

DefaultArgument::DefaultArgument(Token& identifier, Node& expression) 
: identifier(identifier), expression(expression) {}

DefaultArgument::~DefaultArgument() {
    delete &expression;
}

Node* DefaultArgument::construct(Parser& parser) {
    if(not parser.next().of({Token::IDENTIFIER})) return SingleExpression::construct(parser);
    if(not parser.ahead(1).has({"="})) return SingleExpression::construct(parser);
    
    Token& identifier = parser.take();
    parser.take();
    
    return new DefaultArgument(identifier, *SingleExpression::construct(parser));
}

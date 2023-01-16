#include "parser/statements/if-statement.h"
#include "parser/expressions/expression.h"
#include "parser/statements/statement.h"

IfStatement::IfStatement(Node& expression, Node& statement, vector<Node*> others, Node* otherwise)
: expression(expression), statement(statement), others(others), otherwise(otherwise) {}

IfStatement::~IfStatement() {
    delete &expression;
    delete &statement;
    for(Node* node : others) delete node;
    delete otherwise;
}

Nodes IfStatement::nodes() const {
    Nodes nodes = {&expression, &statement};
    nodes.reserve(others.size());

    for(Node* node : others) nodes.push_back(node);
    if(otherwise) nodes.push_back(otherwise);

    return nodes;
}

Node* IfStatement::construct(Parser& parser) {
    if(not parser.next().has({"if"})) return nullptr;
    parser.take();

    Node& expression = *Expression::construct(parser);
    if(parser.next().has({"then"})) parser.take();
    
    Node& statement = *Statement::construct(parser);
    vector<Node*> others;

    while(parser.next().has({"else"}) and parser.ahead(1).has({"if"})) {
        parser.take();
        parser.take();

        Node& expression = *Expression::construct(parser);
        if(parser.next().has({"then"})) parser.take();
        
        Node& statement = *Statement::construct(parser);
        others.push_back(new ElseIf(expression, statement));
    }

    Node* otherwise = nullptr;

    if(parser.next().has({"else"})) {
        parser.take();
        otherwise = Statement::construct(parser);
    }

    return new IfStatement(expression, statement, others, otherwise);
}


ElseIf::ElseIf(Node& expression, Node& statement)
: expression(expression), statement(statement) {}

ElseIf::~ElseIf() {
    delete &expression;
    delete &statement;
}

Nodes ElseIf::nodes() const { return {&expression, &statement}; }

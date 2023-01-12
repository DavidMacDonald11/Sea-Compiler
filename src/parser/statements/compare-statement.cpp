#include "parser/expressions/expression.h"
#include "parser/expressions/single-expression.h"
#include "parser/statements/compare-statement.h"
#include "parser/statements/statement.h"

CompareStatement::CompareStatement(Node& expression, Node* comparator, vector<Node*> withs, Node* otherwise) 
: expression(expression), comparator(comparator), withs(withs), otherwise(otherwise) {}

CompareStatement::~CompareStatement() {
    delete &expression;
    delete comparator;
    for(Node* node : withs) delete node;
    delete otherwise;
}

Nodes CompareStatement::nodes() const {
    Nodes nodes = {&expression};
    if(comparator) nodes.push_back(comparator);
    nodes.reserve(withs.size());

    for(Node* node : withs) nodes.push_back(node);
    if(otherwise) nodes.push_back(otherwise);

    return nodes;
}

Node* CompareStatement::construct() {
    if(not parser->next().has({"compare"})) return nullptr;
    parser->take();

    Node& expression = *Expression::construct();
    Node* comparator = nullptr;

    if(parser->next().has({"using"})) {
        parser->take();
        comparator = SingleExpression::construct();
    }

    if(parser->next().has({"with"})) parser->take();

    Token* brace = (parser->next().has({"{"}))? &parser->take() : nullptr;
    parser->skipNewlines();

    vector<Node*> withs;

    while(not parser->next().has(mergeAll<str>({Token::LINE_ENDS, {"else", "}"}}))) {
        Node& expression = *Expression::construct();

        parser->expectingHas({"?"});
        if(parser->next().has({"then"})) parser->take();

        Node& statement = *Statement::construct();
        withs.push_back(new WithStatement(expression, statement));
    }

    Node* otherwise = nullptr;

    if(parser->next().has({"else"})) {
        parser->take();
        otherwise = Statement::construct();
    }

    if(brace) parser->expectingHas({"}"});
    parser->skipNewlines();

    return new CompareStatement(expression, comparator, withs, otherwise);
}


WithStatement::WithStatement(Node& expression, Node& statement) 
: expression(expression), statement(statement) {}

WithStatement::~WithStatement() {
    delete &expression;
    delete &statement;
}

Nodes WithStatement::nodes() const { return {&expression, &statement}; }

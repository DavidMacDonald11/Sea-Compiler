#include "parser/statements/statement.h"
#include "parser/expressions/expression.h"
#include "util/component.h"

vector<Component*> Statement::nodes() const { return {&statement}; }

Statement::Statement(Node& statement)
: statement(statement) {}

Statement::~Statement() {
    delete &statement;
}

Node* Statement::construct() {
    while(parser->next().has({"\n", ";"})) parser->take();

    Node* node = Expression::construct();
    parser->expectingHas(Token::LINE_ENDS);

    return new Statement(*node);
}

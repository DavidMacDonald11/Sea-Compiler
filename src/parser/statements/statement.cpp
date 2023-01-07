#include "parser/expressions/expression.h"
#include "parser/statements/compound-statement.h"
#include "parser/statements/statement.h"
#include "transpiler/transpiler.h"

vector<Component*> Statement::nodes() const { return {&statement}; }

Statement::Statement(Node& statement)
: statement(statement) {}

Statement::~Statement() {
    delete &statement;
}

Node* Statement::construct() {
    while(parser->next().has({"\n", ";"})) parser->take();

    Node* node = CompoundStatement::construct();
    node = node? node : Expression::construct();

    parser->expectingHas(Token::LINE_ENDS);
    return new Statement(*node);
}

Transpiler::Line Statement::transpile() {
    Transpiler::Line expression = statement.transpile().setShowType();
    return expression.finish(self);
}

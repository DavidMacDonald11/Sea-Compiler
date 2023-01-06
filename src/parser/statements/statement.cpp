#include "parser/statements/statement.h"
#include "parser/expressions/expression.h"
#include "transpiler/transpiler.h"

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

Transpiler::Expression Statement::transpile() {
    Transpiler::Expression expression = statement.transpile().setShowType();
    return expression.finish(self).add("", "\n");
}

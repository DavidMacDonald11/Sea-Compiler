#include "parser/statements/compound-statement.h"
#include "parser/statements/expression-statement.h"
#include "parser/statements/statement.h"

Statement::Statement(Node& statement)
: HiddenStatement(statement) {}

Node* Statement::construct() {
    while(parser->next().has({"\n", ";"})) parser->take();

    Node* node = CompoundStatement::construct();
    node = node? node : ExpressionStatement::construct();

    return new Statement(*node);
}

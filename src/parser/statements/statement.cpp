#include "parser/statements/compound-statement.h"
#include "parser/statements/do-while-statement.h"
#include "parser/statements/line-statement-component.h"
#include "parser/statements/statement.h"
#include "parser/statements/while-statement.h"

Statement::Statement(Node& statement)
: HiddenStatement(statement) {}

Node* Statement::construct() {
    while(parser->next().has({"\n", ";"})) parser->take();
    if(parser->next().has({"EOF"})) return nullptr;

    Node* node = CompoundStatement::construct();
    node = node? node : WhileStatement::construct();
    node = node? node : DoWhileStatement::construct();
    node = node? node : newLineStatement();

    return new Statement(*node);
}

Transpiler::Line Statement::transpile() {
    if(className(&statement) != "LineStatementComponent") return statement.transpile();
    return statement.transpile().finish(self, true);
}

Node* Statement::newLineStatement() {
    Node* node = LineStatementComponent::construct();
    if(parser->context.mustEndLineStatement) parser->expectingHas(Token::LINE_ENDS);
    return node;
}

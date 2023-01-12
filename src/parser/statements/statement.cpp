#include "parser/statements/compound-statement.h"
#include "parser/statements/do-while-statement.h"
#include "parser/statements/for-statement.h"
#include "parser/statements/line-statement-component.h"
#include "parser/statements/statement.h"
#include "parser/statements/while-statement.h"
#include "fault.h"

Statement::Statement(Node& statement)
: HiddenStatement(statement) {}

Node* Statement::construct() {
    while(parser->next().has({"\n", ";"})) parser->take();
    
    if(parser->next().has({"EOF"})) {
        if(parser->context.allowNullStatements) return nullptr;
        throw Fault::fail(parser->take(), "Expecting statement");
    }

    parser->context.allowNullStatements = false;
    Node* node = CompoundStatement::construct();
    node = node? node : WhileStatement::construct();
    node = node? node : DoWhileStatement::construct();
    node = node? node : ForStatement::construct();
    node = node? node : newLineStatement();
    parser->context.allowNullStatements = true;

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

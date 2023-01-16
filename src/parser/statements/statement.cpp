#include "parser/declarations/enum-definition.h"
#include "parser/declarations/function-definition.h"
#include "parser/declarations/struct-definition.h"
#include "parser/statements/c-statement.h"
#include "parser/statements/compare-statement.h"
#include "parser/statements/compound-statement.h"
#include "parser/statements/do-while-statement.h"
#include "parser/statements/for-statement.h"
#include "parser/statements/if-statement.h"
#include "parser/statements/line-statement-component.h"
#include "parser/statements/statement.h"
#include "parser/statements/while-statement.h"

Statement::Statement(Node& statement)
: HiddenStatement(statement) {}

Node* Statement::construct(Parser& parser) {
    while(parser.next().has({"\n", ";"})) parser.take();
    
    if(parser.next().has({"EOF"})) {
        if(parser.context.allowNullStatements) return nullptr;
        throw parser.fault.fail(parser.take(), "Expecting statement");
    }

    parser.context.allowNullStatements = false;
    Node* node = CompoundStatement::construct(parser);
    node = node? node : WhileStatement::construct(parser);
    node = node? node : DoWhileStatement::construct(parser);
    node = node? node : ForStatement::construct(parser);
    node = node? node : IfStatement::construct(parser);
    node = node? node : CompareStatement::construct(parser);
    node = node? node : FunctionDefinition::construct(parser);
    node = node? node : EnumDefinition::construct(parser);
    node = node? node : StructDefinition::construct(parser);
    node = node? node : CStatement::construct(parser);
    node = node? node : newLineStatement(parser);
    parser.context.allowNullStatements = true;

    return new Statement(*node);
}

Transpiler::Line Statement::transpile(Transpiler& transpiler) {
    if(className(&statement) != "LineStatementComponent") return statement.transpile(transpiler);
    return statement.transpile(transpiler).finish(self, transpiler, true);
}

Node* Statement::newLineStatement(Parser& parser) {
    parser.context.allowNullStatements = false;
    Node* node = LineStatementComponent::construct(parser);
    if(parser.context.mustEndLineStatement) parser.expectingHas(Token::LINE_ENDS);
    parser.context.allowNullStatements = true;
    
    return node;
}

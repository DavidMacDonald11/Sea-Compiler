#include "parser/statements/do-while-statement.h"
#include "parser/expressions/expression.h"
#include "parser/statements/statement.h"

DoWhileStatement::DoWhileStatement(Token* label, Node& statement, Node& condition)
: label(label), statement(statement), condition(condition) {}

DoWhileStatement::~DoWhileStatement() {
    delete &statement;
    delete &condition;
}

Nodes DoWhileStatement::nodes() const {
    return label? Nodes{label, &statement, &condition} :
        Nodes{&statement, &condition};
}

Node* DoWhileStatement::construct() {
    if(not parser->nextOrAfterHas({"do"})) return nullptr;

    Token* label = (parser->next().of({Token::IDENTIFIER}))? &parser->take() : nullptr;
    parser->expectingHas({"do"});
    parser->skipNewlines();

    parser->context.mustEndLineStatement = false;
    Node* statement = Statement::construct();
    parser->context.mustEndLineStatement = true;

    parser->skipNewlines();
    parser->expectingHas({"while"});

    Node* condition = Expression::construct();
    parser->expectingHas(Token::LINE_ENDS);
    
    return new DoWhileStatement(label, *statement, *condition);
}

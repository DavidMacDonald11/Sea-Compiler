#include "parser/statements/while-statement.h"
#include "parser/expressions/expression.h"
#include "parser/statements/statement.h"

WhileStatement::WhileStatement(Token* label, Node& condition, Node& statement)
: label(label), condition(condition), statement(statement) {}

WhileStatement::~WhileStatement() {
    delete &condition;
    delete &statement;
}

Nodes WhileStatement::nodes() const {
    return label? Nodes{label, &condition, &statement} :
        Nodes{&condition, &statement};
}

Node* WhileStatement::construct() {
    if(not parser->nextOrAfterHas({"while"})) return nullptr;

    Token* label = (parser->next().of({Token::IDENTIFIER}))? &parser->take() : nullptr;
    parser->expectingHas({"while"});

    Node* expression = Expression::construct();
    parser->skipNewlines();

    if(parser->next().has({"do"})) parser->take();
    Node* statement = Statement::construct();

    return new WhileStatement(label, *expression, *statement);
}

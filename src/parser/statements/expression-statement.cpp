#include "parser/expressions/expression.h"
#include "parser/statements/expression-statement.h"
#include "parser/statements/hidden-statement.h"

ExpressionStatement::ExpressionStatement(Node& expression) 
: HiddenStatement(expression) {}

Node* ExpressionStatement::construct() {
    Node* node = Expression::construct();
    parser->expectingHas(Token::LINE_ENDS);
    return node;
}

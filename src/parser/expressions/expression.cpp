#include "parser/expressions/expression.h"
#include "parser/expressions/conditional-expression.h"

Node* Expression::construct() {
    return ConditionalExpression::construct();
}

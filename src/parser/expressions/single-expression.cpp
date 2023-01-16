#include "parser/expressions/single-expression.h"
#include "parser/expressions/conditional-expression.h"

Node* SingleExpression::construct(Parser& parser) {
    return ConditionalExpression::construct(parser);
}
#include "parser/expressions/conditional-expression.h"
#include "parser/expressions/expression.h"
#include "parser/expressions/logical-or-expression.h"

vector<Component*> ConditionalExpression::nodes() const {
    return {&left, &condition, &right};
}

ConditionalExpression::ConditionalExpression(Node& left, Node& condition, Node& right)
: left(left), condition(condition), right(right) {}

ConditionalExpression::~ConditionalExpression() {
    delete &left;
    delete &condition;
    delete &right;
}

Node* ConditionalExpression::construct(Parser& parser) {
    Node* node = LogicalOrExpression::construct(parser);
    if(not parser.next().has({"if"})) return node;

    parser.take();
    Node& condition = *Expression::construct(parser);
    parser.expectingHas({"else"});
    
    return new ConditionalExpression(*node, condition, *ConditionalExpression::construct(parser));
}

Transpiler::Line ConditionalExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line left = self.left.transpile(transpiler);
    Transpiler::Line condition = self.condition.transpile(transpiler);
    Transpiler::Line right = self.right.transpile(transpiler);
    Transpiler::Line result = Transpiler::Line::resolve(left, right);
    
    return result.replace(fmt::format("{1}? {0} : {2}", 
        left.toString(), condition.toString(), right.toString()));
}

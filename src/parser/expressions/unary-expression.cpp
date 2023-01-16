#include <map>
#include "parser/declarations/type-name.h"
#include "parser/expressions/unary-expression.h"
#include "parser/expressions/postfix-expression.h"

std::map<str, str> opMap {
    {"+", "+"},
    {"-", "-"},
    {"!", "~"},
    {"^", "&"},
    {"@", "*"}
};


vector<Component*> UnaryExpression::nodes() const {
    return {&op, &expression};
}

UnaryExpression::UnaryExpression(Token& op, Node& expression)
: op(op), expression(expression) {}

UnaryExpression::~UnaryExpression() {
    delete &expression;
}

Node* UnaryExpression::construct(Parser& parser) {
    Token& next = parser.next();

    if(next.has(Token::PREFIX_UNARY_OPS) or next.has({"await"})) {
        Token& op = parser.take();
        return new UnaryExpression(op, *UnaryExpression::construct(parser));
    }

    if(not next.has({"sizeof", "alignof"})) return PostfixExpression::construct(parser);
    Token& op = parser.take();

    if(not parser.next().has({"("})) 
        return new UnaryExpression(op, *UnaryExpression::construct(parser));

    if(not parser.ahead(1).has(Token::TYPE_NAME_KEYWORDS)) 
        return new UnaryExpression(op, *UnaryExpression::construct(parser));
    
    parser.take();
    Node* expression = TypeName::construct(parser);
    parser.expectingHas({")"});

    return new UnaryExpression(op, *expression);
}

Transpiler::Line UnaryExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line line = expression.transpile(transpiler);

    if(op.has({"sizeof", "alignof"})) {
        return line.add(op.string + "(", ")").cast("c::size_t");
    }
    
    if(op.has({"^"})) line.pointers++;
    else if(op.has({"@"})) line.pointers -= line.pointers? 1 : 0;

    return line.add(opMap[op.string]);
}

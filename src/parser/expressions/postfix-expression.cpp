#include "parser/expressions/primary-expression.h"
#include "parser/expressions/postfix-expression.h"

vector<Component*>  PostfixExpression::nodes() const {
    return {&expression, &op, &identifier};
}

PostfixExpression::PostfixExpression(Node& expression, Token& op, Token& identifier) 
: expression(expression), op(op), identifier(identifier) {}

PostfixExpression::~PostfixExpression() {
    delete &expression;
}

Node* PostfixExpression::construct() {
    Node* node = PrimaryExpression::construct();

    while(parser->next().has(Token::POSTFIX_OPS)) {
        Token& op = parser->take(); 
        Token& identifier = parser->expectingOf({Token::IDENTIFIER});

        node = new PostfixExpression(*node, op, identifier);
    }

    return node;
}

Transpiler::Line PostfixExpression::transpile() {
    Transpiler::Line left = expression.transpile();
    str op = left.pointers? "->" : ".";
    
    if(self.op.has({"::"})) 
        return left.add("", "_" + identifier.string);

    Transpiler::Line result = (className(&expression) != "PostfixExpression")? 
        left : static_cast<PostfixExpression*>(&expression)->staticTranspile();

    result.add("", op + identifier.string);
    if(self.op.has({"."})) return result; 

    result.add("? ", " : 0");
    return result.add(left.toString()).add("(", ")");
}

Transpiler::Line PostfixExpression::staticTranspile() {
    Transpiler::Line left = expression.transpile();
    str op = left.pointers? "->" : ".";

    if(self.op.has({"::"})) 
        return left.add("", "_" + identifier.string);

    Transpiler::Line result = left;
    result.add("", op + identifier.string);
    return result; 
}

// TODO call expression

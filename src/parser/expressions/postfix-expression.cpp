#include "parser/expressions/expression.h"
#include "parser/expressions/postfix-call-expression.h"
#include "parser/expressions/primary-expression.h"
#include "parser/expressions/postfix-expression.h"

Nodes  PostfixExpression::nodes() const {
    return {&expression, &op, &identifier};
}

PostfixExpression::PostfixExpression(Node& expression, Token& op, Token& identifier) 
: expression(expression), op(op), identifier(identifier) {}

PostfixExpression::~PostfixExpression() {
    delete &expression;
}

Node* PostfixExpression::construct() {
    Node* node = PrimaryExpression::construct();

    while(parser->next().has({".", "?.", "[", "("})) {
        Token& op = parser->take(); 

        if(op.has({"["})) {
            Node* newNode = PostfixIndexExpression::construct();
            static_cast<PostfixIndexExpression*>(newNode)->expression = node;
            node = newNode;
            continue;
        }

        if(op.has({"("})) {
            Node* newNode = PostfixCallExpression::construct();
            static_cast<PostfixCallExpression*>(newNode)->expression = node;
            node = newNode;
            continue;
        }

        Token& identifier = parser->expectingOf({Token::IDENTIFIER});
        node = new PostfixExpression(*node, op, identifier);    
    }

    return node;
}

Transpiler::Line PostfixExpression::transpile() {
    Transpiler::Line left = expression.transpile();
    str op = left.pointers? "->" : ".";

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

    Transpiler::Line result = left;
    result.add("", op + identifier.string);
    return result; 
}


Nodes PostfixIndexExpression::nodes() const {
    return {expression, &index};
}

PostfixIndexExpression::PostfixIndexExpression(Node& index)
: expression(nullptr), index(index) {}

PostfixIndexExpression::~PostfixIndexExpression() {
    delete expression;
    delete &index;
}

Node* PostfixIndexExpression::construct() {
    Node* index = Expression::construct();
    parser->expectingHas({"]"});
    return new PostfixIndexExpression(*index);
}

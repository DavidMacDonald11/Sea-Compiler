#include "parser/expressions/expression.h"
#include "parser/expressions/postfix-call-expression.h"
#include "parser/expressions/primary-expression.h"
#include "parser/expressions/postfix-expression.h"
#include "transpiler/transpiler.h"

template<class T>
static Node* constructExternalNode(Parser& parser, Node* node) {
    Node* newNode = T::construct(parser);
    static_cast<T*>(newNode)->expression = node;
    return newNode;
}


Nodes  PostfixExpression::nodes() const {
    return {&expression, &op, &identifier};
}

PostfixExpression::PostfixExpression(Node& expression, Token& op, Token& identifier) 
: expression(expression), op(op), identifier(identifier) {}

PostfixExpression::~PostfixExpression() {
    delete &expression;
}

Node* PostfixExpression::construct(Parser& parser) {
    Node* node = PrimaryExpression::construct(parser);

    while(parser.next().has({".", "?.", "[", "(", "istype"})) {
        Token& op = parser.take(); 

        if(op.has({"["})) {
            node = constructExternalNode<PostfixIndexExpression>(parser, node);
            continue;
        }

        if(op.has({"("})) {
            node = constructExternalNode<PostfixCallExpression>(parser, node);
            continue;
        }

        Token& identifier = parser.expectingOf({Token::IDENTIFIER});
        node = new PostfixExpression(*node, op, identifier);    
    }

    return node;
}

Transpiler::Line PostfixExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line left = expression.transpile(transpiler);
    str op = left.pointers? "->" : ".";

    Transpiler::Line result = (className(&expression) != "PostfixExpression")? 
        left : static_cast<PostfixExpression*>(&expression)->staticTranspile(transpiler);

    result.add("", op + identifier.string);
    if(self.op.has({"."})) return result; 

    result.add("? ", " : 0");
    return result.add(left.toString()).add("(", ")");
}

Transpiler::Line PostfixExpression::staticTranspile(Transpiler& transpiler) {
    Transpiler::Line left = expression.transpile(transpiler);
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

Node* PostfixIndexExpression::construct(Parser& parser) {
    Node* index = Expression::construct(parser);
    parser.expectingHas({"]"});
    return new PostfixIndexExpression(*index);
}

Transpiler::Line PostfixIndexExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line result = expression->transpile(transpiler);
    Transpiler::Line index = self.index.transpile(transpiler);

    index.add("[", "]");
    return result.add("", index.toString());
}

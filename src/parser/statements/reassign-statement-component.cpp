#include "parser/statements/reassign-statement-component.h"
#include "parser/expressions/expression.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "fault.h"

ReassignStatementComponent::ReassignStatementComponent(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* ReassignStatementComponent::construct(Node& left) {
    Token& op = parser->expectingHas(Token::ASSIGN_OPS);
    Node* right = Expression::construct();

    if(not parser->next().has(Token::ASSIGN_OPS)) 
        return new ReassignStatementComponent(left, op, *right);
        
    if(not op.has({"="})) throw Fault::fail(op, fmt::format(
        "Cannot chain '{}' reassignment", op.string));

    if(not parser->next().has({"="})) {
        Token& op = parser->take();

        throw Fault::fail(op, fmt::format("Cannot chain '{}' reassignment", 
            op.string));
    }

    return new ReassignStatementComponent(left, op, 
        *ReassignStatementComponent::construct(*right));
}

Transpiler::Line ReassignStatementComponent::transpile() {
    str op = self.op.string;
    if(op == "$=") op = "^=";

    Transpiler::Line line = left.transpile();
    line.add("", fmt::format(" {} {}", op, right.transpile().toString()));

    return line;
}

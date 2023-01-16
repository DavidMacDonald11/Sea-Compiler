#include "parser/statements/reassign-statement-component.h"
#include "parser/expressions/expression.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"

ReassignStatementComponent::ReassignStatementComponent(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* ReassignStatementComponent::construct(Parser& parser, Node& left) {
    Token& op = parser.expectingHas(Token::ASSIGN_OPS);
    Node* right = Expression::construct(parser);

    if(not parser.next().has(Token::ASSIGN_OPS)) 
        return new ReassignStatementComponent(left, op, *right);
        
    if(not op.has({"="})) throw parser.fault.fail(op, fmt::format(
        "Cannot chain '{}' reassignment", op.string));

    if(not parser.next().has({"="})) {
        Token& op = parser.take();

        throw parser.fault.fail(op, fmt::format("Cannot chain '{}' reassignment", 
            op.string));
    }

    return new ReassignStatementComponent(left, op, 
        *ReassignStatementComponent::construct(parser, *right));
}

Transpiler::Line ReassignStatementComponent::transpile(Transpiler& transpiler) {
    str op = self.op.string;
    if(op == "$=") op = "^=";

    Transpiler::Line line = left.transpile(transpiler);
    line.add("", fmt::format(" {} {}", op, right.transpile(transpiler).toString()));

    return line;
}

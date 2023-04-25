package sea.grammar

import sea.parser.*

data class UnaryExpression(val op: Token, val expression: Node): Node() {
    override val parts: Parts = listOf(op, expression)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(!parser.next.has(Token.PREFIX_UNARY_OPS)) {
                return PostfixExpression.construct(parser)
            }

            val op = parser.take()
            val node = UnaryExpression.construct(parser)

            return if(op.has("&", "$")) OwnershipExpression(op, node) else UnaryExpression(op, node)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        if(op.has("!")) throw NotImplementedError()

        return transpiler.nodeContext(this) {
            val expression = expression.transpile(transpiler).arithmeticOp(transpiler)

            if(op.has("+")) expression else {
                expression.add("-").castReplace("Int")
                if("Nat" in expression.type) expression.castReplace("Int") else expression
            }
         }
    }
}


data class OwnershipExpression(val op: Token, val expression: Node): Node() {
    override val parts: Parts = listOf(op, expression)

    override fun transpile(transpiler: Transpiler): TExpression {
        if(expression !is Identifier)
            throw transpiler.faults.fail(this, "Cannot transfer/borrow non-identifier")

        if(op.has("&")) throw NotImplementedError()

        val expression = expression.transpile(transpiler)
        expression.transfer = Pair(op.string, this.expression.token.string)

        return expression
    }
}

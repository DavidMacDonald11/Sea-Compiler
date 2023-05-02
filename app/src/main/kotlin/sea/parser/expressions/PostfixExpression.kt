package sea.grammar

import sea.parser.*

abstract class PostfixExpression: Node() {
    abstract var expression: Node?

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            var node = PrimaryExpression.construct(parser)

            while(parser.next.has(Token.POSTFIX_OPS)) {
                val newNode = NotNullExpression.construct(parser)
                (newNode as PostfixExpression).expression = node
                node = newNode
            }

            return node
        }
    }
}


data class NotNullExpression(val token: Token): PostfixExpression() {
    override var expression: Node? = null

    override val parts: Parts get() = listOf(expression, token)

    override fun tree(prefix: String): String {
        return "${this::class.simpleName} ── $expression $token"
    }

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return NotNullExpression(parser.expectingHas("!!"))
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = expression!!.transpile(transpiler)

        if(!result.type.nullable) {
            transpiler.faults.warn(this, "Unnecessary not-null assertion")
            return result
        }

        if(result.isNull) {
            transpiler.faults.error(this, "'null' cannot be asserted not-null")
            return result
        }

        result.type.nullable = false
        return result.add("(", ").value")
    }
}

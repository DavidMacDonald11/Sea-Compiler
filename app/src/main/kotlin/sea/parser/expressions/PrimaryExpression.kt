package sea.grammar

import sea.parser.*

abstract class PrimaryExpression: Node() {
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.next.of(TokenType.NUM)) return Number.construct(parser)
            if(parser.next.has("(")) return ParentheseseExpression.construct(parser)

            val token = parser.take()
            throw parser.faults.fail(token, "PrimaryExpression error; unexpected token $token")
        }
    }
}


class Number(token: Token, val imag: Token?): PrimaryNode(token) {
    override val parts: Parts = listOf(token, imag).filterNotNull()

    override fun tree(prefix: String): String {
        return "${this::class.simpleName} ── $token" + if(imag != null) " $imag" else ""
    }

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val num = parser.take()
            val imag = if(parser.next.has("i")) parser.take() else null
            if(imag != null) imag.type = TokenType.PUNC

            return Number(num, imag)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val expression = TExpression("", token.string)

        if(imag != null) expression.cast("Imag").add(after = "j")
        else expression.cast(if("." in token.string) "Real" else "Nat")

        return expression
    }
}

class ParentheseseExpression(val expression: Node): Node() {
    override val parts: Parts = listOf(expression)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            parser.expectingHas("(")
            parser.skipNewlines()

            val expression = Expression.construct(parser)

            parser.skipNewlines()
            parser.expectingHas(")")

            return ParentheseseExpression(expression)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        return expression.transpile(transpiler).add("(", ")")
    }
}

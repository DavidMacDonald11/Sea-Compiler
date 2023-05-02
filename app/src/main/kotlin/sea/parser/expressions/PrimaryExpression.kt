package sea.grammar

import sea.parser.*
import sea.transpiler.symbols.Value

abstract class PrimaryExpression: Node() {
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.next.of(TokenType.NUM)) return Number.construct(parser)
            if(parser.next.of(TokenType.IDENTIFIER)) return Identifier.construct(parser)
            if(parser.next.has(Token.PRIMARY_KEYWORDS)) return PrimaryKeyword.construct(parser)
            if(parser.next.has("(")) return ParentheseseExpression.construct(parser)
            if(parser.next.has("[")) return ArrayInitializer.construct(parser)

            val token = parser.take()
            throw parser.faults.fail(token, "PrimaryExpression error; unexpected token $token")
        }
    }
}


class Number(token: Token, val imag: Token?): PrimaryNode(token) {
    override val parts: Parts = listOf(token, imag)

    override fun tree(prefix: String): String {
        return "${this::class.simpleName} ── $token" + if(imag != null) " $imag" else ""
    }

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val num = parser.expectingOf(TokenType.NUM)
            val imag = if(parser.next.has("i")) parser.take() else null
            if(imag != null) imag.type = TokenType.PUNC

            return Number(num, imag)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val expression = TExpression("", token.string)

        if(imag != null) expression.cast("Cplex").add(after = "j")
        else expression.cast(if("." in token.string) "Real" else "Int")

        return expression
    }
}


class Identifier(token: Token): PrimaryNode(token) {
    override val parts: Parts = listOf(token)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return Identifier(parser.expectingOf(TokenType.IDENTIFIER))
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        return transpiler.nodeContext(this) {
            val name = token.string
            val symbol = transpiler.symbols[name]
            (symbol as Value).access(transpiler)
        }
    }
}


class PrimaryKeyword(token: Token): PrimaryNode(token) {
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            return PrimaryKeyword(parser.expectingHas(Token.PRIMARY_KEYWORDS))
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        if(token.string in listOf("true", "false")) {
            return TExpression("Bool", token.string)
        }

        if(token.string in listOf("infinity", "nan")) {
            transpiler.include("tgmath")
            return TExpression("Real", if(token.has("nan")) "NAN" else "INFINITY")
        }

        val result = TExpression(TType(nullable = true))
        result.isNull = true
        return result
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

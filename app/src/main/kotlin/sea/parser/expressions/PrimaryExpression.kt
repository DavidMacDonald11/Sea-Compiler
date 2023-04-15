package sea.grammar

import sea.lexer.Token
import sea.parser.*

abstract class PrimaryExpression : Node() {
    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.next.of(Token.Type.NUM)) return NumericConstant.construct(parser)

            val token = parser.take()
            throw parser.faults.fail(token, "PrimaryExpression error; unexpected token $token")
        }
    }
}


class NumericConstant(token: Token, val imag: Token?) : PrimaryNode(token) {
    override val parts: Parts = listOf(token, imag).filterNotNull()

    override fun tree(prefix: String): String {
        return "${this::class.simpleName} ── $token" + if(imag != null) " $imag" else ""
    }

    companion object : Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val num = parser.take()
            val imag = if(parser.next.has("i")) parser.take() else null
            if(imag != null) imag.type = Token.Type.PUNC

            return NumericConstant(num, imag)
        }
    }
}


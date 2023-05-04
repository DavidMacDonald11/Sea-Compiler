package sea.grammar

import sea.parser.*

data class TypeName(val token: Token, val nullable: Token?): Node() {
    override val parts: Parts = listOf(token, nullable)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            if(parser.next.has("[")) return ArrayType.construct(parser)

            val token = parser.expectingHas(Token.TYPE_KEYWORDS)
            val nullable = if(parser.next.has("?")) parser.take() else null

            return TypeName(token, nullable)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = TExpression(TType(token.string))
        result.type.nullable = (nullable != null)
        return result
    }
}


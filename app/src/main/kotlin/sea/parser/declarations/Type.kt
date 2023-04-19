package sea.grammar

import sea.parser.*

data class Type(val dynamic: Token?, val name: Token, val nullable: Token?): Node() {
    override val parts: Parts = listOf(dynamic, name, nullable)
    
    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val dynamic = if(parser.next.has("#")) parser.take() else null
            val name = parser.expectingHas(Token.TYPE_KEYWORDS)
            val nullable = if(parser.next.has("?")) parser.take() else null

            return Type(dynamic, name, nullable)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val type = TType(name.string, (dynamic != null), (nullable != null))
        return TExpression(type, "__sea_type_${name.string}__")
    }
}

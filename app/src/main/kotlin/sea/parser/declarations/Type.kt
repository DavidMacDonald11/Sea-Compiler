package sea.grammar

import sea.parser.*

data class Type(val dynamic: Token?, val type: Node, val nullable: Token?): Node() {
    override val parts: Parts = listOf(dynamic, type, nullable)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val dynamic = if(parser.next.has("#")) parser.take() else null
            val type = TypeName.construct(parser)
            val nullable = if(parser.next.has("?")) parser.take() else null

            return Type(dynamic, type, nullable)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = type.transpile(transpiler)
        result.type.dynamic = (dynamic != null)
        result.type.nullable = (nullable != null)

        return result.replace(result.type.cName)
    }
}

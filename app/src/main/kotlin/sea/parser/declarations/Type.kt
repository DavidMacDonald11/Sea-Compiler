package sea.grammar

import sea.parser.*

data class Type(val dynamic: Token?, val type: Node): Node() {
    override val parts: Parts = listOf(dynamic, type)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val dynamic = if(parser.next.has("#")) parser.take() else null
            val type = TypeName.construct(parser)

            return Type(dynamic, type)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = type.transpile(transpiler)
        result.type.dynamic = (dynamic != null)

        return result.replace(result.type.cName)
    }
}

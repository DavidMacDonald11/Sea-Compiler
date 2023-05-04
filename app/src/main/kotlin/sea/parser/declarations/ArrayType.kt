package sea.grammar

import sea.parser.*

data class ArrayType(val size: Node, val type: Node, val nullable: Token?): Node() {
    override val parts: Parts = listOf(size, type)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            parser.expectingHas("[")
            val size = Expression.construct(parser)
            parser.expectingHas("of")
            val type = TypeName.construct(parser)
            parser.expectingHas("]")

            val nullable = if(parser.next.has("?")) parser.take() else null
            return ArrayType(size, type, nullable)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val size = size.transpile(transpiler)
        val result = type.transpile(transpiler)

        var type = TType("Array")
        type.arrayType = result.type
        type.nullable = (nullable != null)

        if(size.longValue == null || size.longValue!! < 1L) {
            transpiler.faults.error(this.size, "Array size must be constant positive integer value")
            type.arraySize = 0
        } else type.arraySize = size.longValue

        return TExpression(type)
    }
}

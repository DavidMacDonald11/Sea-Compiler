package sea.grammar

import sea.parser.*

data class ArrayType(val type: Node): Node() {
    override val parts: Parts = listOf(type)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            parser.expectingHas("Array")
            parser.expectingHas("<")
            val type = TypeName.construct(parser)
            parser.expectingHas(">")

            return ArrayType(type)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = type.transpile(transpiler)

        var type = TType("Array")
        type.arrayType = result.type

        return TExpression(type)
    }
}

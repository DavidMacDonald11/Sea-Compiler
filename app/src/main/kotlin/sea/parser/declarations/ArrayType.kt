package sea.grammar

import sea.parser.*

data class ArrayType(val type: Node, val size: Node?): Node() {
    override val parts: Parts = listOf(type, size)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            parser.expectingHas("Array")
            parser.expectingHas("<")

            val type = TypeName.construct(parser)
            var size: Node? = null

            if(parser.next.has(",")) {
                parser.take()
                size = Expression.construct(parser)
            }

            parser.expectingHas(">")
            return ArrayType(type, size)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = type.transpile(transpiler)

        var type = TType("Array")
        type.arrayType = result.type

        if(size != null) {
            val expr = size.transpile(transpiler).castUp()

            if(!expr.isConstant)
                transpiler.faults.error(size, "Array size must be constant")

            if("Nat" !in expr.type && "Int" !in expr.type)
                transpiler.faults.error(size, "Array size must be an integer")

            type.arraySize = 0 // TODO get actual value
        }

        return TExpression(type)
    }
}

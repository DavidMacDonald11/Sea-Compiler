package sea.grammar

import sea.parser.*

data class ArrayInitializer(val tokens: Pair<Token, Token>, val expressions: List<Node>): Node() {
    override val parts: Parts = listOf(tokens.first) + expressions + listOf(tokens.second)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            val first = parser.expectingHas("[")
            parser.skipNewlines()

            val expressions = ArrayList<Node>()

            while(!parser.next.has("]")) {
                expressions.add(Expression.construct(parser))

                if(parser.next.has(",")) parser.take()
                parser.skipNewlines()
            }

            val second = parser.expectingHas("]")
            return ArrayInitializer(Pair(first, second), expressions)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val oldType = transpiler.context.assignType
        var eType = TType()

        if(transpiler.context.assignType != null) {
            val aType = transpiler.context.assignType!!
            transpiler.context.assignType = aType.arrayType
            eType = aType.arrayType ?: TType()
        }

        var items = ArrayList<TExpression>()

        for(it in expressions) {
            val expression = it.transpile(transpiler)

            eType = TType.resolve(eType, expression.type).copy()
            if(expression.type.nullable) eType.nullable = true

            if(eType.arrayDim != expression.type.arrayDim) {
                transpiler.faults.error(this, "Cannot mismatch array dimensions")
                break
            }

            if(eType.arraySize != expression.type.arraySize) {
                transpiler.faults.error(this, "Cannot mismatch array sizes")
                break
            }

            items.add(expression)
        }

        var type = TType("Array")
        type.arrayType = eType
        type.arraySize = items.size.toLong()

        var elems = items.joinToString(", ") {
            if(!eType.nullable) it.string else {
                if(it.isNull) "{true}"
                else if(!it.type.nullable) "{false, ${it.string}}"
                else "{(${it.string}).isNull, (${it.string}).value}"
            }
        }

        val result = TExpression(type, "{$elems}")
        result.isConstant = items.all { it.isConstant }

        transpiler.context.assignType = oldType
        return result
    }
}

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
        var eType = TType()
        var items = ArrayList<TExpression>()

        for(it in expressions) {
            val expression = it.transpile(transpiler)

            eType = TType.resolve(eType, expression.type).copy()
            if(expression.type.nullable) eType.nullable = true

            if(eType.arrayDim != expression.type.arrayDim) {
                transpiler.faults.error(this, "Cannot mismatch array dimensions")
                break
            }

            items.add(expression)
        }

        // TODO fix 2d+ array type
        // TODO heap array

        var type = TType("Array")
        type.arrayType = eType

        var elems = items.joinToString(", ") {
            if(!eType.nullable) it.string else {
                if(it.isNull) "{true}"
                else if(!it.type.nullable) "{false, ${it.string}}"
                else "{(${it.string}).isNull, (${it.string}).value}"
            }
        }

        elems = "(${eType.cName}[]){$elems}"
        val result = TExpression(type, "(__sea_type_Array__){${items.size}, $elems}")
        result.isConstant = items.all { it.isConstant }

        return result
    }
}

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
        var result = ""

        expressions.forEach {
            val expression = it.transpile(transpiler)
            eType = TType.resolve(eType, expression.type)

            if(TExpression.realAndImag(TExpression(eType), expression))
                eType.string = eType.string.replaceFirst("[a-zA-Z]+".toRegex(), "Cplex")

            if(result != "") result += ", "
            result += "${expression.string}"
        }

        // TODO drop imag
        // TODO nullables
        // TODO list of expr, then cast after type deduced

        var type = TType("Array")
        type.arrayType = eType
        type.arraySize = expressions.size.toLong()

        return TExpression(type, "{ $result }")
    }
}

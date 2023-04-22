package sea.grammar

import sea.parser.*

data class CastExpression(val expression: Node, val type: Node): Node() {
    override val parts: Parts = listOf(expression, type)

    companion object: Node.CompanionObject {
        override fun construct(parser: Parser): Node {
            var node = AdditiveExpression.construct(parser)

            while(parser.next.has("as")) {
                parser.take()
                node = CastExpression(node, Type.construct(parser))
            }

            return node
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        val result = expression.transpile(transpiler)

        if("None" in result.type.string) {
            transpiler.faults.error(this, "Cannot cast from None-type")
            return result
        }

        val type = type.transpile(transpiler)

        if(!result.type.nullable && type.type.nullable) {
            transpiler.faults.error(this, "Cannot cast non-nullable into nullable type")
            return result
        }

        if(result.type.nullable) {
            if(type.type.nullable) {
                result.delayedCast = result.delayedCast ?: TExpression(string = result.string)
            }
        
            result.add("*(", ")")
        }

        val cName = type.type.cName

        if("Imag" in type.type) {
            if("Imag" in result.type) 
                return result.dropImag().add("($cName)(", ") * 1j").cast(type.type)

            if("Cplex" !in result.type) 
                return result.replace("($cName)0").cast(type.type)

            val name = result.type.cName.replace("Cplex", "Real")
            val thing = result.string

            return result.add("($cName)(((", ") - ($name)($thing)) / 1j) * 1j").cast(type.type)
        }

        if("Imag" in result.type && "Cplex" !in type.type) {
            return result.add("($cName)(", ")").cast(type.type)
        }

        return result.add("($cName)(", ")")
    }
}

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

        internal fun castValue(result: TExpression, type: TType): TExpression {
            val cType = type.rawCName
            if("Imag" !in type) return result.add("($cType)(", ")").cast(type)

            val cplexType = cType.replace("Imag", "Cplex")

            if("Imag" in result.type) return result.add("($cplexType)(", ")").cast(type)
            if("Cplex" !in result.type) return result.replace("($cplexType)0").cast(type)

            val realType = result.type.cName.replace("Cplex", "Real")
            val name = result.string

            return result.replace("($cplexType)(($name) - ($realType)($name))").cast(type)
        }
    }

    override fun transpile(transpiler: Transpiler): TExpression {
        var result = expression.transpile(transpiler)

        if("None" in result.type.string) {
            transpiler.faults.error(this, "Cannot cast from 'None' type")
            return result
        }

        val type = type.transpile(transpiler).type
        val cType = type.cName

        if(type.dynamic) result.type.dynamic = true
        if(result.type == type) return result

        if(!result.type.nullable && type.nullable) {
            return result.add("($cType){false, ", "}").cast(type)
        }

        if(result.isNull) {
            if(!type.nullable)
                transpiler.faults.error(this, "Cannot cast null to non-nullable type")

            return result.cast(type)
        }

        if(!result.type.nullable) return castValue(result, type)

        if(!type.nullable) result.type.nullable = false
        val name = result.string

        result.add(after = ".value")
        if("Imag" in result.type) result.add(after = " * 1j")
        result = castValue(result, type)

        return result.add("($cType){$name.isNull, ", "}")
    }
}

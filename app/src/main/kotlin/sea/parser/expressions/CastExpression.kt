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
        var result = expression.transpile(transpiler)

        if("None" in result.type) {
            transpiler.faults.error(this, "Cannot cast None type")
            return result
        }

        if("Array" in result.type) {
            transpiler.faults.error(this, "Cannot cast Array type")
            return result
        }

        val type = type.transpile(transpiler).type
        val cType = type.cName

        if("Array" in type) {
            transpiler.faults.error(this, "Cannot cast to Array type")
        }

        if(type.dynamic) result.type.dynamic = true
        if(result.type == type) return result

        if(result.isNull) {
            if(!type.nullable)
                transpiler.faults.error(this, "Cannot cast null to non-nullable type")

            return result.cast(type)
        }

        if(!result.type.nullable) {
            if(!type.nullable) return result.add("($cType)(", ")").cast(type)
            return result.add("($cType){false, ", "}").cast(type)
        }

        val name = result.string
        return result.replace("($cType){($name).isNull, ($name).value}").cast(type)
    }
}
